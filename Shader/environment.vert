  #version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec3 Position;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    Position = vec3(model * vec4(position, 1.0));    // ����������ϵ��ָ��
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalMatrix * normal; // ���㷨��������ģ�ͱ任��ֵ
}