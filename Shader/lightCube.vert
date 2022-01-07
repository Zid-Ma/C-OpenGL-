#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;//������
//��ʹ�÷���Ч��ʱ��������ɫ������ӣ���ֹ�ǵƹ���ɫ����
//layout (location = 1) out vec4 BrightColor;//����һ��������ɫ��д�뵽�ĸ���ɫ���壺

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}