#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

//反射
void main()
{             
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R), 1.0);
}

//折射
//void main()
//{
//    float indexRation = 1.00 / 1.52;
//    vec3 viewDir = normalize(Position - cameraPos); // 注意这里向量从观察者位置指出
//    vec3 refractDir = refract(viewDir, normalize(Normal), indexRation);
//    FragColor = texture(skybox, refractDir);   // 使用 折射向量 采样环境纹理
//}



