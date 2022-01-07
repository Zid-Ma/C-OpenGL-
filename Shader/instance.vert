#version 330 core

//GLSL在顶点着色器中嵌入了另一个内建变量，gl_InstanceID
//在使用实例化渲染调用时，gl_InstanceID会从0开始，在每个实例被渲染时递增1。
//因为每个实例都有唯一的ID，我们可以建立一个数组，将ID与位置值对应起来，将每个实例放置在世界的不同位置。
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec2 aOffset;//将偏移设置为实例化数组

//out vec3 fColor;
out vec2 TexCoords;//需要输出的参数，纹理位置

uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

void main()
{
    //gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
    gl_Position = projection * view * model * vec4((aPos.x + aOffset.x), aPos.y, (aPos.z + aOffset.y), 1.0f);//最终渲染的顶点位置参数
	TexCoords = aTexCoords;
    //fColor = aColor;
}
/*
out vec3 fColor;

//包含100个偏移向量
uniform vec2 offsets[100];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(aPos + offset, 0.0, 1.0);
    fColor = aColor;
}
*/