#version 330 core
/*第一种方法-将当前片段的法线向量进行转换
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

uniform sampler2D normalMap;

void main()
{
    //更新法线贴图代码，引入切线到世界空间变换：
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    //变换法线到当前世界空间
    normal = normalize(fs_in.TBN * normal);
}
*/

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform bool normalMapping;

//视差贴图
//---------
uniform sampler2D depthMap;
uniform float height_scale;
//返回经位移的纹理坐标(fragment的纹理坐标,切线空间中的fragment到观察者的方向向量)
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);


void main()
{           
    //法线贴图
    //--------
    //从[0,1]范围内的法线贴图获取法线
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    //将法向量变换为范围[-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // 获得漫反射颜色
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // 环境
    vec3 ambient = 0.1 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;

    //视差贴图
    //--------
    // Offset texture coordinates with Parallax Mapping//使用视差贴图偏移纹理坐标
    vec3 viewDir   = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
    //超出默认纹理坐标范围进行采样的时候就丢弃片段，防止失真（一般应用于平面）
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    discard;
    // then sample textures with new texture coords//然后使用新的纹理坐标对纹理进行采样
    vec3 diffuse = texture(diffuseMap, texCoords);
    vec3 normal  = texture(normalMap, texCoords);
    normal = normalize(normal * 2.0 - 1.0);//这里获取法线的方法与上面第二种获取法线贴图的方法一样
    //继续执行照明规范
    [--....--]
    // Get diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;
    // Ambient
    vec3 ambient = 0.1 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
     vec3 specular = vec3(0.2) * spec;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height =  texture(depthMap, texCoords).r;    
    //相比正朝向表面，当带有角度地看向平面时，我们会更大程度地缩放的大小，从而增加纹理坐标的偏移；这样做在视角上会获得更大的真实度
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);//当viewDir大致平行于表面时，它的z元素接近于0.0，除法会返回比viewDir垂直于表面的时候更大的向量
    return texCoords - p;    
}

//陡峭视差映射（使用到的参数与视差贴图一样）
//------------通过增加采样的数量提高了精确性//当遇到锯齿效果以及图层之间有明显的断层时通过增加样本的方式减少，或者不使用低于表面的第一个位置，而是在两个接近的深度层进行插值找出更匹配的样本（地形视差图 和 视差遮蔽贴图技术）
vec2 ParallaxMapping-SteepParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    //先定义层的数量，计算每一层的深度，最后计算纹理坐标偏移，每一层我们必须沿着的方向进行移动。

    //深度层的数量 number of depth layers
    //const float numLayers = 10;
    //(深度层数量计算升级)当垂直看一个表面的时候纹理时位移比以一定角度看时的小。我们可以在垂直看时使用更少的样本，以一定角度看时增加样本数量：
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));//得到viewDir和正z方向的点乘，使用它的结果根据看向表面的角度调整样本数量（注意正z方向等于切线空间中的表面的法线）。如果所看的方向平行于表面，我们就是用32层。
    //计算每层的大小 calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    //当前层的深度 depth of current layer
    float currentLayerDepth = 0.0;
    //每层纹理坐标的偏移量（从矢量P开始） the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
    float deltaTexCoords = P / numLayers;


    //获取初始值 get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
    //然后遍历所有层，从上开始，直到找到小于这一层的深度值的深度贴图值：
    while(currentLayerDepth < currentDepthMapValue)
    {
        //沿P方向移动纹理坐标 shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        //获取当前纹理坐标处的深度贴图值 get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        //获取下一层的深度 get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    return texCoords - currentTexCoords;     
}


//视差遮蔽映射(Parallax Occlusion Mapping)
//----------------------------------------和陡峭视差映射的原则相同，但不是用触碰的第一个深度层的纹理坐标，而是在触碰之前和之后，在深度层之间进行线性插值，根据表面的高度距离啷个深度层的深度层值的距离来确定线性插值的大小
vec2 ParallaxMapping-ParallaxOcclusionMapping(vec2 texCoords, vec3 viewDir)
{ 
    //深度层的数量 number of depth layers
    const float minLayers = 10;
    const float maxLayers = 20;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    //计算每层的大小 calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    //当前层的深度 depth of current layer
    float currentLayerDepth = 0.0;
    //每层纹理坐标的偏移量（从矢量P开始） the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    //获取初始值 get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        //沿P方向移动纹理坐标 shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        //获取当前纹理坐标处的深度贴图值 get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        //获取下一层的深度 get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // --从此处开始的视差遮挡贴图插值 parallax occlusion mapping interpolation from here on
    ///在碰撞前获取纹理坐标（反向操作） get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    //获取线性插值碰撞前后的深度 get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    //纹理坐标插值 interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}