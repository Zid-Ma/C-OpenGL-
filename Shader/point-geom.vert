#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    vs_out.color = aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}
/*
#version 330 core
layout (location = 0) in vec2 aPos;

void main()
{
    //顶点着色器只需要在z平面绘制点就可以了，所以我们将使用一个最基本顶点着色器
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}
*/