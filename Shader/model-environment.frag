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

        uniform samplerCube envText;    // 环境纹理
        uniform sampler2D texture_diffuse1;//texture_diffuse0;
        uniform sampler2D texture_specular1;//specular_diffuse0;
        //uniform sampler2D texture_normal1; 
        //uniform sampler2D texture_height1;// 反射map//texture_reflection0; // 反射map
        //uniform sampler2D texture_reflection1;
        uniform vec3 cameraPos;



        out vec4 FragColor;

        void main()
        {
            //漫反射颜色
            vec4    diffuseColor = texture(texture_diffuse1, TexCoords);//texture_diffuse//material.diffuse
            //反射量
            float   relefctIntensity = texture(texture_specular1, TexCoords).r;//texture_height//material.height
            //初始化反射颜色
            vec4    reflectColor = vec4(0.0, 0.0, 0.0, 0.0);
            //判断映射量觉得是否启用环境映射
            if (relefctIntensity > 0.1) // 决定是否开启//relefctIntensity > 0.1
            {
                //先计算观察方向向量
                vec3 viewDir = normalize(FragPos - cameraPos);//观察位置减去摄像机位置
                //映射方向向量//reflect()计算映射位置
                vec3 reflectDir = reflect(viewDir, normalize(FragNormal));//material.normal//FragNormal
                //得到映射过后的颜色
                reflectColor = texture(envText, reflectDir) * 1.0; // 使用反射向量采样环境纹理 使用强度系数控制
            }
            //将映射颜色与漫反射颜色相加
            FragColor = diffuseColor + reflectColor;
        }