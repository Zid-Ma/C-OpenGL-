#version 330 core
/*��һ�ַ���-����ǰƬ�εķ�����������ת��
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

uniform sampler2D normalMap;

void main()
{
    //���·�����ͼ���룬�������ߵ�����ռ�任��
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    //�任���ߵ���ǰ����ռ�
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

//�Ӳ���ͼ
//---------
uniform sampler2D depthMap;
uniform float height_scale;
//���ؾ�λ�Ƶ���������(fragment����������,���߿ռ��е�fragment���۲��ߵķ�������)
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);


void main()
{           
    //������ͼ
    //--------
    //��[0,1]��Χ�ڵķ�����ͼ��ȡ����
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    //���������任Ϊ��Χ[-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // �����������ɫ
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ����
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

    //�Ӳ���ͼ
    //--------
    // Offset texture coordinates with Parallax Mapping//ʹ���Ӳ���ͼƫ����������
    vec3 viewDir   = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
    //����Ĭ���������귶Χ���в�����ʱ��Ͷ���Ƭ�Σ���ֹʧ�棨һ��Ӧ����ƽ�棩
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    discard;
    // then sample textures with new texture coords//Ȼ��ʹ���µ����������������в���
    vec3 diffuse = texture(diffuseMap, texCoords);
    vec3 normal  = texture(normalMap, texCoords);
    normal = normalize(normal * 2.0 - 1.0);//�����ȡ���ߵķ���������ڶ��ֻ�ȡ������ͼ�ķ���һ��
    //����ִ�������淶
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
    //�����������棬�����нǶȵؿ���ƽ��ʱ�����ǻ����̶ȵ����ŵĴ�С���Ӷ��������������ƫ�ƣ����������ӽ��ϻ��ø������ʵ��
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);//��viewDir����ƽ���ڱ���ʱ������zԪ�ؽӽ���0.0�������᷵�ر�viewDir��ֱ�ڱ����ʱ����������
    return texCoords - p;    
}

//�����Ӳ�ӳ�䣨ʹ�õ��Ĳ������Ӳ���ͼһ����
//------------ͨ�����Ӳ�������������˾�ȷ��//���������Ч���Լ�ͼ��֮�������ԵĶϲ�ʱͨ�����������ķ�ʽ���٣����߲�ʹ�õ��ڱ���ĵ�һ��λ�ã������������ӽ�����Ȳ���в�ֵ�ҳ���ƥ��������������Ӳ�ͼ �� �Ӳ��ڱ���ͼ������
vec2 ParallaxMapping-SteepParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    //�ȶ���������������ÿһ�����ȣ���������������ƫ�ƣ�ÿһ�����Ǳ������ŵķ�������ƶ���

    //��Ȳ������ number of depth layers
    //const float numLayers = 10;
    //(��Ȳ�������������)����ֱ��һ�������ʱ������ʱλ�Ʊ���һ���Ƕȿ�ʱ��С�����ǿ����ڴ�ֱ��ʱʹ�ø��ٵ���������һ���Ƕȿ�ʱ��������������
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));//�õ�viewDir����z����ĵ�ˣ�ʹ�����Ľ�����ݿ������ĽǶȵ�������������ע����z����������߿ռ��еı���ķ��ߣ�����������ķ���ƽ���ڱ��棬���Ǿ�����32�㡣
    //����ÿ��Ĵ�С calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    //��ǰ������ depth of current layer
    float currentLayerDepth = 0.0;
    //ÿ�����������ƫ��������ʸ��P��ʼ�� the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
    float deltaTexCoords = P / numLayers;


    //��ȡ��ʼֵ get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
    //Ȼ��������в㣬���Ͽ�ʼ��ֱ���ҵ�С����һ������ֵ�������ͼֵ��
    while(currentLayerDepth < currentDepthMapValue)
    {
        //��P�����ƶ��������� shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        //��ȡ��ǰ�������괦�������ͼֵ get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        //��ȡ��һ������ get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    return texCoords - currentTexCoords;     
}


//�Ӳ��ڱ�ӳ��(Parallax Occlusion Mapping)
//----------------------------------------�Ͷ����Ӳ�ӳ���ԭ����ͬ���������ô����ĵ�һ����Ȳ���������꣬�����ڴ���֮ǰ��֮������Ȳ�֮��������Բ�ֵ�����ݱ���ĸ߶Ⱦ���ॸ���Ȳ����Ȳ�ֵ�ľ�����ȷ�����Բ�ֵ�Ĵ�С
vec2 ParallaxMapping-ParallaxOcclusionMapping(vec2 texCoords, vec3 viewDir)
{ 
    //��Ȳ������ number of depth layers
    const float minLayers = 10;
    const float maxLayers = 20;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    //����ÿ��Ĵ�С calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    //��ǰ������ depth of current layer
    float currentLayerDepth = 0.0;
    //ÿ�����������ƫ��������ʸ��P��ʼ�� the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    //��ȡ��ʼֵ get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        //��P�����ƶ��������� shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        //��ȡ��ǰ�������괦�������ͼֵ get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        //��ȡ��һ������ get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // --�Ӵ˴���ʼ���Ӳ��ڵ���ͼ��ֵ parallax occlusion mapping interpolation from here on
    ///����ײǰ��ȡ�������꣨��������� get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    //��ȡ���Բ�ֵ��ײǰ������ get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    //���������ֵ interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}