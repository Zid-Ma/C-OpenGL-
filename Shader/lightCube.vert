#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;//法向量
//当使用泛光效果时可以在着色器中添加（不止是灯光着色器）
//layout (location = 1) out vec4 BrightColor;//控制一个像素着色器写入到哪个颜色缓冲：

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}