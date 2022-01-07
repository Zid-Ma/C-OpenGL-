#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;//需要接收的参数,纹理位置


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragNormal;
out vec3 FragPos;//输出将用来在片段着色器内计算观察方向向量
out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords; //输出到片段着色器中的纹理位置参数
    gl_Position = projection * view * model * vec4(position, 1.0);
    //Position输出向量是一个世界空间的位置向量
    // 在世界坐标系中指定
    FragPos = vec3(model * vec4(position, 1.0));    
    //我们现在使用了一个法向量，所以我们将再次使用法线矩阵(Normal Matrix)来变换它们。
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    // 计算法向量经过模型变换后值
    FragNormal = normalMatrix * normal; 
}
