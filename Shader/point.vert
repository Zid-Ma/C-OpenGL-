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

//����Ĵ�С����Ϊ�ü��ռ�λ�õ�zֵ
void main()
{
    //���ͱ���gl_VertexID���������ڻ��ƶ���ĵ�ǰID

    gl_Position = projection * view * model * vec4(aPos, 1.0);    
    gl_PointSize = gl_Position.z;    
}

//������һ������vs_out�Ľӿڿ飬�����������ϣ�����͵���һ����ɫ���е������������
out VS_OUT
{
    vec2 TexCoords;
} vs_out;

//Uniform�������(Uniform Buffer Object)
//ʹ��һ���򵥵Ķ�����ɫ������projection��view����洢����ν��Uniform��(Uniform Block)��
//������һ������Matrices��Uniform�飬������������4x4����Uniform���еı�������ֱ�ӷ��ʣ�����Ҫ�ӿ�����Ϊǰ׺
layout (std140) uniform Matrices//layout (std140)���������˼��˵����ǰ�����Uniform�����������ʹ��һ���ض����ڴ沼��
{
    mat4 projection;
    mat4 view;
};
//Uniform��������Ǵ�����һ����������еģ���ʵ����ֻ��һ��Ԥ���ڴ档��Ϊ����ڴ沢���ᱣ�������屣�����ʲô���͵����ݣ����ǻ���Ҫ����OpenGL�ڴ����һ���ֶ�Ӧ����ɫ���е���һ��uniform����
//������ɫ���������µ����Uniform��
//std140����������ÿ��������ƫ����������һϵ�й����������ģ�����ʽ��������ÿ���������͵��ڴ沼�֡�����������ʽ�ἰ�ģ����ǿ����ֶ������ÿ��������ƫ������
//ÿ����������һ����׼������(Base Alignment)��������һ��������Uniform������ռ�ݵĿռ䣨���������(Padding)���������׼��������ʹ��std140���ֵĹ����������ġ�
//����������ÿ�������������ټ������Ķ���ƫ����(Aligned Offset)������һ�������ӿ���ʼλ�õ��ֽ�ƫ������һ�������Ķ����ֽ�ƫ����������ڻ�׼�������ı�����
/*
���ֹ����ԭ�Ŀ�����OpenGL��Uniform����淶�����ҵ��������ǽ����������г�����Ĺ���GLSL�е�ÿ������������˵int��float��bool����������Ϊ4�ֽ�����ÿ4���ֽڽ�����һ��N����ʾ��

����	            ���ֹ���
������              ����int��bool	ÿ�������Ļ�׼������ΪN��
����	            2N����4N������ζ��vec3�Ļ�׼������Ϊ4N��
����������������	ÿ��Ԫ�صĻ�׼��������vec4����ͬ��
����	            ����Ϊ�����������飬ÿ�������Ļ�׼��������vec4����ͬ��
�ṹ��	            ��������Ԫ�ظ��ݹ�������Ĵ�С��������䵽vec4��С�ı�����
*/
layout (std140) uniform ExampleBlock//����std140���ֻ���shared���ֻ�packed
{
    float value;
    vec3  vector;
    mat4  matrix;
    float values[3];
    bool  boolean;
    int   integer;
};
/*
layout (std140) uniform ExampleBlock
{
                     // ��׼������       // ����ƫ����
    float value;     // 4               // 0 
    vec3 vector;     // 16              // 16  (������16�ı��������� 4->16)
    mat4 matrix;     // 16              // 32  (�� 0)
                     // 16              // 48  (�� 1)
                     // 16              // 64  (�� 2)
                     // 16              // 80  (�� 3)
    float values[3]; // 16              // 96  (values[0])
                     // 16              // 112 (values[1])
                     // 16              // 128 (values[2])
    bool boolean;    // 4               // 144
    int integer;     // 4               // 148
};

//��OpenGL 4.2�汾����Ҳ�������һ�����ֱ�ʶ������ʽ�ؽ�Uniform��İ󶨵㴢������ɫ���У������Ͳ����ٵ���glGetUniformBlockIndex��glUniformBlockBinding�ˡ�
layout(std140, binding = 2) uniform Lights { ... }; 
*/