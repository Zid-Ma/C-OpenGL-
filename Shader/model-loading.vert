#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}




//layout (location = 0) in vec3 aPos;//��Ҫ���յĲ���,λ��
//layout (location = 1) in vec3 aNormal;//������
//layout (location = 2) in vec2 aTexCoords;//��Ҫ���յĲ���,����λ��

//out vec2 TexCoords;//��Ҫ����Ĳ���������λ��

//uniform mat4 model;//ģ�;���
//uniform mat4 view;//�۲����
//uniform mat4 projection;//ͶӰ����

//void main()
//{
//	gl_Position = projection * view * model * vec4(aPos, 1.0f);//������Ⱦ�Ķ���λ�ò���
//	TexCoords = vec2(aTexCoords.x, aTexCoords.y);//�����Ƭ����ɫ���е�����λ�ò���
//}