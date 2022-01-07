#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    vs_out.color = aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}
/*
#version 330 core
layout (location = 0) in vec2 aPos;

void main()
{
    //������ɫ��ֻ��Ҫ��zƽ����Ƶ�Ϳ����ˣ��������ǽ�ʹ��һ�������������ɫ��
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}
*/