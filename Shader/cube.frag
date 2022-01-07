#version 330 core
out vec4 FragColor;

//����
struct Material {
    vec3 ambient;//����
    vec3 diffuse;//������
    vec3 specular;//����
    float shininess;//�����
}; 
//��Դ�������Ӱ��
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//����ǿ��
float specularStrength = 0.5;

in vec2 TexCoord;//��������
in vec3 FragPos;//Ƭ��λ��
in vec3 Normal;//������

//���������//texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;//������ɫ

uniform vec3 lightPos;//��Դλ��
uniform vec3 viewPos;//�۲���λ��

uniform Material material;
uniform Light light;

void main()
{
	//���������
	//----------
	//������ǿ��
	//float ambientStrength = 0.1;
	//material.ambient = ambientStrength;
	//������
	vec3 ambient = light.ambient * material.ambient;
	//�������Ӻû������Ľ��

	//��������ռ���
	//--------------
	vec3 norm = normalize(Normal);//���б�׼������Ϊ��λ����
	vec3 lightDir = normalize(lightPos - FragPos);
	//��norm��lightDir�������е�ˣ������Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��
	float diff = max(dot(norm, lightDir), 0.0);//ʹ��max����������������֮��ϴ�Ĳ������Ӷ���֤��������������ɸ���
	//���Թ����ɫ���õ����������(��������֮��ĽǶ�Խ������������ͻ�ԽС)
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//������ռ���
	//------------
	//���߷�������
	vec3 viewDir = normalize(viewPos - FragPos);
	//��Ӧ�����ŷ�����ķ�������
	vec3 reflectDir = reflect(-lightDir, norm);//(�ӹ�Դָ��Ƭ��λ�õ�����,�ѱ�׼���ķ�����)
	//�����������
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//�ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess)
	vec3 specular = light.specular * (spec * material.specular);

	//���Ϲ���+����Ļ�ϼ���
	//-----------------------
	vec3 result = (ambient + diffuse + specular) * objectColor;
	//FragColor = vec4(result, 1.0);//�����
	//����������֮�����Բ�ֵ��80%������20%������// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(result,1.0) * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}