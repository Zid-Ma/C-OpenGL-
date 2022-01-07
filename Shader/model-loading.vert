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




//layout (location = 0) in vec3 aPos;//需要接收的参数,位置
//layout (location = 1) in vec3 aNormal;//法向量
//layout (location = 2) in vec2 aTexCoords;//需要接收的参数,纹理位置

//out vec2 TexCoords;//需要输出的参数，纹理位置

//uniform mat4 model;//模型矩阵
//uniform mat4 view;//观察矩阵
//uniform mat4 projection;//投影矩阵

//void main()
//{
//	gl_Position = projection * view * model * vec4(aPos, 1.0f);//最终渲染的顶点位置参数
//	TexCoords = vec2(aTexCoords.x, aTexCoords.y);//输出到片段着色器中的纹理位置参数
//}