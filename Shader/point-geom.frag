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
    //��Ƭ����ɫ����Ӳ���룬�����еĵ㶼���Ϊ��ɫ
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);   
}
*/