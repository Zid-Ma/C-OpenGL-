#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    //定向光/点光源--位置
    vec3 position;

    //环境光
    vec3 direction;

    //冯氏光照
    //--------
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //点光源
    //------
    float constant;
    float linear;
    float quadratic;
};

//聚光
struct Spotlight{
    vec3  position;
    vec3  direction;
    float cutOff;
    ...
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	//环境光计算
	//----------
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//漫反射光照计算
	//--------------
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  

	//镜面光照计算
	//------------
    //冯氏光照
    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //Blinn-Phong光照
    //vec3 lightDir   = normalize(light.position - FragPos);
    //vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);//获取半程向量的方法很简单，只需要将光线的方向向量和观察向量加到一起，并将结果正规化(Normalize)就可以了
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//面光分量的实际计算只不过是对表面法线和半程向量进行一次约束点乘(Clamped Dot Product)，让点乘结果不为负，从而获取它们之间夹角的余弦值，之后我们对这个值取反光度次方
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

	//冯氏光照计算
	//-----------------------
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
	//FragColor = vec4(result,1.0) * mix(texture(material.diffuse, TexCoord), texture(material.specular, TexCoord), 0.2);
}