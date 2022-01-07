#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;//��-����

in VS_OUT {
    vec3 normal;
} gs_in[];

//����
const float MAGNITUDE = 0.4;

//������ɫ�������ÿһ�����㣨����һ��λ��������һ����������������ÿ��λ������������һ����������
void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;//λ������λ��
    EmitVertex();
    //ͨ������������λ������������ڶ���λ��
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;//��������������һ��MAGNITUDE��������������ʾ���ķ�������С
    EmitVertex();
    EndPrimitive();
}

void main()
{
    //�����ε���������
    GenerateLine(0); // ��һ�����㷨��
    GenerateLine(1); // �ڶ������㷨��
    GenerateLine(2); // ���������㷨��
}