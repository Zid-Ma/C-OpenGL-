#version 330 core
out vec4 FragColor;

//材质
struct Material {
    vec3 ambient;//环境
    vec3 diffuse;//漫反射
    vec3 specular;//镜面
    float shininess;//反光度
}; 
//光源对物体的影响
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//镜面强度
float specularStrength = 0.5;

in vec2 TexCoord;//纹理坐标
in vec3 FragPos;//片段位置
in vec3 Normal;//法向量

//纹理采样器//texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;//物体颜色

uniform vec3 lightPos;//光源位置
uniform vec3 viewPos;//观察者位置

uniform Material material;
uniform Light light;

void main()
{
	//环境光计算
	//----------
	//环境光强度
	//float ambientStrength = 0.1;
	//material.ambient = ambientStrength;
	//环境光
	vec3 ambient = light.ambient * material.ambient;
	//计算出添加好环境光后的结果

	//漫反射光照计算
	//--------------
	vec3 norm = normalize(Normal);//进行标准化，作为单位向量
	vec3 lightDir = normalize(lightPos - FragPos);
	//对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫发射影响
	float diff = max(dot(norm, lightDir), 0.0);//使用max函数返回两个参数之间较大的参数，从而保证漫反射分量不会变成负数
	//乘以光的颜色，得到漫反射分量(两个向量之间的角度越大，漫反射分量就会越小)
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//镜面光照计算
	//------------
	//视线方向向量
	vec3 viewDir = normalize(viewPos - FragPos);
	//对应的沿着法线轴的反射向量
	vec3 reflectDir = reflect(-lightDir, norm);//(从光源指向片段位置的向量,已标准化的法向量)
	//镜面分量计算
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)
	vec3 specular = light.specular * (spec * material.specular);

	//冯氏光照+纹理的混合计算
	//-----------------------
	vec3 result = (ambient + diffuse + specular) * objectColor;
	//FragColor = vec4(result, 1.0);//最后结果
	//在两种纹理之间线性插值（80%容器，20%人脸）// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(result,1.0) * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}