#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;//��Ҫ���յĲ���,����λ��


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragNormal;
out vec3 FragPos;//�����������Ƭ����ɫ���ڼ���۲췽������
out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords; //�����Ƭ����ɫ���е�����λ�ò���
    gl_Position = projection * view * model * vec4(position, 1.0);
    //Position���������һ������ռ��λ������
    // ����������ϵ��ָ��
    FragPos = vec3(model * vec4(position, 1.0));    
    //��������ʹ����һ�����������������ǽ��ٴ�ʹ�÷��߾���(Normal Matrix)���任���ǡ�
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    // ���㷨��������ģ�ͱ任��ֵ
    FragNormal = normalMatrix * normal; 
}
