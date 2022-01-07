        #version 330 core

        in vec3 FragNormal;
        in vec3 FragPos;
        in vec2 TexCoords;

//struct Material {
    //sampler2D diffuse;
    //sampler2D specular;
    //sampler2D normal;
    //sampler2D height;
    //sampler2D reflection;
    //float shininess;
//}; 
//uniform Material material;

        uniform samplerCube envText;    // ��������
        uniform sampler2D texture_diffuse1;//texture_diffuse0;
        uniform sampler2D texture_specular1;//specular_diffuse0;
        //uniform sampler2D texture_normal1; 
        //uniform sampler2D texture_height1;// ����map//texture_reflection0; // ����map
        //uniform sampler2D texture_reflection1;
        uniform vec3 cameraPos;



        out vec4 FragColor;

        void main()
        {
            //��������ɫ
            vec4    diffuseColor = texture(texture_diffuse1, TexCoords);//texture_diffuse//material.diffuse
            //������
            float   relefctIntensity = texture(texture_specular1, TexCoords).r;//texture_height//material.height
            //��ʼ��������ɫ
            vec4    reflectColor = vec4(0.0, 0.0, 0.0, 0.0);
            //�ж�ӳ���������Ƿ����û���ӳ��
            if (relefctIntensity > 0.1) // �����Ƿ���//relefctIntensity > 0.1
            {
                //�ȼ���۲췽������
                vec3 viewDir = normalize(FragPos - cameraPos);//�۲�λ�ü�ȥ�����λ��
                //ӳ�䷽������//reflect()����ӳ��λ��
                vec3 reflectDir = reflect(viewDir, normalize(FragNormal));//material.normal//FragNormal
                //�õ�ӳ��������ɫ
                reflectColor = texture(envText, reflectDir) * 1.0; // ʹ�÷������������������� ʹ��ǿ��ϵ������
            }
            //��ӳ����ɫ����������ɫ���
            FragColor = diffuseColor + reflectColor;
        }