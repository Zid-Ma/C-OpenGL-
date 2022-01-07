#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    //�����/���Դ--λ��
    vec3 position;

    //������
    vec3 direction;

    //���Ϲ���
    //--------
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //���Դ
    //------
    float constant;
    float linear;
    float quadratic;
};

//�۹�
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
	//���������
	//----------
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//��������ռ���
	//--------------
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  

	//������ռ���
	//------------
    //���Ϲ���
    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //Blinn-Phong����
    //vec3 lightDir   = normalize(light.position - FragPos);
    //vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

	//���Ϲ��ռ���
	//-----------------------
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
	//FragColor = vec4(result,1.0) * mix(texture(material.diffuse, TexCoord), texture(material.specular, TexCoord), 0.2);
}