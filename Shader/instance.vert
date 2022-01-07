#version 330 core

//GLSL�ڶ�����ɫ����Ƕ������һ���ڽ�������gl_InstanceID
//��ʹ��ʵ������Ⱦ����ʱ��gl_InstanceID���0��ʼ����ÿ��ʵ������Ⱦʱ����1��
//��Ϊÿ��ʵ������Ψһ��ID�����ǿ��Խ���һ�����飬��ID��λ��ֵ��Ӧ��������ÿ��ʵ������������Ĳ�ͬλ�á�
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec2 aOffset;//��ƫ������Ϊʵ��������

//out vec3 fColor;
out vec2 TexCoords;//��Ҫ����Ĳ���������λ��

uniform mat4 model;//ģ�;���
uniform mat4 view;//�۲����
uniform mat4 projection;//ͶӰ����

void main()
{
    //gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
    gl_Position = projection * view * model * vec4((aPos.x + aOffset.x), aPos.y, (aPos.z + aOffset.y), 1.0f);//������Ⱦ�Ķ���λ�ò���
	TexCoords = aTexCoords;
    //fColor = aColor;
}
/*
out vec3 fColor;

//����100��ƫ������
uniform vec2 offsets[100];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(aPos + offset, 0.0, 1.0);
    fColor = aColor;
}
*/