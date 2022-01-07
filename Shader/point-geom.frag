#version 330 core
out vec4 FragColor;

in vec3 fColor;

void main()
{
    FragColor = vec4(fColor, 1.0);   
}
/*
#version 330 core
out vec4 FragColor;

void main()
{
    //在片段着色器中硬编码，将所有的点都输出为绿色
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);   
}
*/