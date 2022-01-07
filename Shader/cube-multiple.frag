#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 FragNormal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

//�����
//------
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

//���Դ
//------
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
//�����ĸ���������ݵ�PointLight�ṹ��
#define NR_POINT_LIGHTS 4

//�۹�Դ
//------
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform vec3 viewPos;
uniform samplerCube envTexture;
uniform sampler2D shadowMap;
//uniform samplerCube depthMap;//��ʹ��������Ӱӳ��ʱ����Ҫ�õ�����������ͼ
//uniform float far_plane;//ͬʱ��ʹ��������Ӱӳ��ʱ����Ҫӳ����׶ƽ���Զ��ֵ
//uniform sampler2D normalMap;//������ͼ
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

//��������
//--------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);
float ShadowCalculation(vec3 fragPos);//��ʹ��������Ӱӳ��ʱ����Ҫ�õ�����Ӱ���㺯��

bool blinn = true;

void main()
{   
    //�Ƿ�����blinn-phongģ��
    blinn = true;

    // ����
    //-----
    vec3 norm = normalize(Normal);
    //�Ƿ����÷�����ͼ
    if(false)
    {
        // �ӷ�����ͼ��Χ[0,1]��ȡ����
        //norm = texture(normalMap, TexCoords).rgb;
        // ����������ת��Ϊ��Χ[-1,1]
        //norm = normalize(normal * 2.0 - 1.0);
    }
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 viewDirX = normalize(FragPos - viewPos);//�۲�λ�ü�ȥ�����λ��

    // ��һ�׶Σ��������
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // �ڶ��׶Σ����Դ
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // �����׶Σ��۹�
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    //����ӳ��
    //---------
    //��ʼ��������ɫ
    vec4    reflectColor = vec4(0.0, 0.0, 0.0, 1.0);
    //������
    float   relefctIntensity = result.r;//texture_height//material.height
    //����
    if(true)
    {
    //�ж�ӳ���������Ƿ����û���ӳ��
        if (true) // �����Ƿ���//relefctIntensity > 0.1
        {
            if(!blinn)
            {
                //���Ͼ���ӳ��
                //------------
                //�ȼ���۲췽������
                viewDirX = normalize(FragPos - viewPos);//�۲�λ�ü�ȥ�����λ��
                //ӳ�䷽������//reflect()����ӳ��λ��
                vec3 reflectDir = reflect(viewDirX, normalize(FragNormal));//material.normal//FragNormal
                //�õ�ӳ��������ɫ
                reflectColor = texture(envTexture, reflectDir); // ʹ�÷������������������� ʹ��ǿ��ϵ������
            }
            else
            {
                //Blinn-Phong����
                //vec3 lightDir   = normalize(light.position - FragPos);
                //vec3 viewDir    = normalize(FragPos - viewPos);//�۲�λ�ü�ȥ�����λ��
                vec3 lightDir = normalize(spotLight.position - FragPos);
                vec3 halfwayDir = normalize(lightDir + viewDirX);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
                float reflectDir = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
                //�õ�ӳ��������ɫ
                reflectColor = texture(envTexture, result); // ʹ�÷������������������� ʹ��ǿ��ϵ������
            }
        }
    }
    //����
    if(false)
    {
    //������
    float indexRation = 1.00 / 1.52;
    vec3 viewDirZ = normalize(FragPos - viewPos); // ע�����������ӹ۲���λ��ָ��
    vec3 refractDir = refract(viewDirZ, normalize(FragNormal), indexRation);
    reflectColor = texture(envTexture, refractDir);   // ʹ�� �������� ������������
    }

    // ������Ӱ
    //----------
    //�����һ��shadowֵ����fragment����Ӱ��ʱ��1.0������Ӱ����0.0
    float shadow = ShadowCalculation(FragPosLightSpace);//�ɹ���ӽǽ��м��㵱ǰƬ���Ƿ�����Ӱ�У������������ռ�����
    int xxx = 0;
    if(shadow == 0.0)xxx = 1;

    // Reinhardɫ��ӳ��
    //------------------
    result = result / (result + vec3(1.0));//����㷨����������������ģ���������᲻��ô��ϸҲ����ô�����ֶȡ�
    // �ع�ɫ��ӳ��
    //float exposure = 1.0;//�ع�ֵ(0.1, 1.0, 5.0)//���ع�ֵ��ʹ����ĺڰ�������ʾ�����ϸ�ڣ�Ȼ�����ع�ֵ���������ٺڰ������ϸ�ڣ����������ǿ����������������ϸ��
    //result = vec3(1.0) - exp(-result * exposure);
    // GammaУ��
    //const float gamma = 2.2f;
    //result = pow(result, vec3(1.0 / gamma));

    //����

    FragColor = vec4(result * (0.5 + 1 - xxx), 1.0);//vec4(result, 1.0) + reflectColor * relefctIntensity; + vec4(reflectColor.rgb, 1.0)
}

//����⺯��
//----------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //��Դ�����׼��
    vec3 lightDir = normalize(-light.direction);
    // ��������ɫ
    // -----------
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    // -----------
    float spec = 0.0f;
    if(!blinn)
    {
        //���Ϲ���
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong����
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
    }


    // �ϲ����
    //---------
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

//���Դ����
//----------
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //��Դ����
    vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0f;
    if(!blinn)
    {
        //���Ϲ���
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong����
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
    }
    // ˥��
    // -----
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    

    // �ϲ����
    //---------
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

//�۹�Դ����
//-----------
// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    // specular shading
    // -----------
    float spec = 0.0f;
    if(!blinn)
    {
        //���Ϲ���
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong����
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
    }
    // attenuation
    // ------------
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    // --------------------
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    // ----------------
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

//��Ӱ����
//--------
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // ִ��͸�ӳ���:�ѹ�ռ�Ƭ��λ��ת��Ϊ���пռ�ı�׼���豸����
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;//���ڲ��пռ��FragPosLightSpace������ͨ��gl_Position����Ƭ����ɫ������Ǳ����Լ���͸�ӳ�����������Ƭ���ڹ�ռ��-1��1�ķ�Χ
    // �任��[0,1]�ķ�Χ
    projCoords = projCoords * 0.5 + 0.5;
    // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
    float closestDepth = texture(shadowMap, projCoords.xy).r; //�õ����λ����Ұ����������
    // ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
    float currentDepth = projCoords.z;
    //ʹ����Ӱƫ��
    float bias = 0.005;
    // ��鵱ǰƬ���Ƿ�����Ӱ��
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    //FragColor = vec4(vec3(shadow), 1.0);

    return shadow;
}

/*
//��Ӱ����-������Ӱӳ��
//--------
float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPos; 
    //closestDepth�ǹ�Դ������ӽ��Ŀɼ�fragment֮��ı�׼�������ֵ��
    float closestDepth = texture(depthMap, fragToLight).r;//������������Ҫ�ǵ�λ��������������������б�׼��
    //closestDepthֵ������0��1�ķ�Χ���ˣ����������Ƚ���ת����0��far_plane�ķ�Χ������Ҫ��������far_plane��
    closestDepth *= far_plane;
    //��ȡ��ǰfragment�͹�Դ֮������ֵ�����ǿ��Լ򵥵�ʹ��fragToLight�ĳ�������ȡ������ȡ����������μ�����������ͼ�е����ֵ��
    float currentDepth = length(fragToLight);
    //�������ǿ��Խ��������ֵ�Ա�һ�£�������һ�����ӽ����Դ˾�����ǰ��fragment�Ƿ�����Ӱ���С����ǻ�Ҫ����һ����Ӱƫ�ƣ����Բ��ܱ�����Ӱʧ�棬
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}
*/
//һ���򵥵İ���Ȼ�����ʾ�����ļ��ɣ���ShadowCalculation�����м����׼����closestDepth�������ѱ�����ʾΪ��
//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);