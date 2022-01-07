#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
    //�����߱任���ü��ռ�����֮ǰ����ʹ�÷��߾���任һ�Σ�������ɫ�����ܵ�λ�������Ǽ��ÿռ����꣬��������Ӧ�ý��������任����ͬ�Ŀռ��У�
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));
}