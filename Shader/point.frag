#version 330 core
layout (depth_<condition>) out float gl_FragDepth;//在片段着色器的顶部使用深度条件(Depth Condition)重新声明gl_FragDepth变量：
/*condition可以为下面的值：

条件	描述
any	默认值。提前深度测试是禁用的，你会损失很多性能
greater	你只能让深度值比gl_FragCoord.z更大
less	你只能让深度值比gl_FragCoord.z更小
unchanged	如果你要写入gl_FragDepth，你将只能写入gl_FragCoord.z的值
*/

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{             
    //gl_FrontFacing变量是一个bool，如果当前片段是正向面的一部分那么就是true，否则就是false
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoords);
    else
        FragColor = texture(backTexture, TexCoords);

    //在着色器内设置片段的深度值。
    gl_FragDepth = 0.0; // 这个片段现在的深度值为 0.0//如果着色器没有写入值到gl_FragDepth，它会自动取用gl_FragCoord.z的值。
    //只要我们在片段着色器中对gl_FragDepth进行写入，OpenGL就会（像深度测试小节中讨论的那样）禁用所有的提前深度测试(Early Depth Testing)。
    //它被禁用的原因是，OpenGL无法在片段着色器运行之前得知片段将拥有的深度值，因为片段着色器可能会完全修改这个深度值。
}

/*
//对片段的深度值进行了递增，但仍然也保留了一些提前深度测试：

#version 420 core // 注意GLSL的版本！
out vec4 FragColor;
layout (depth_greater) out float gl_FragDepth;

void main()
{             
    FragColor = vec4(1.0);
    gl_FragDepth = gl_FragCoord.z + 0.1;
}
*/

//定义一个输入接口块。块名(Block Name)应该是和着色器中一样的（VS_OUT），但实例名(Instance Name)(实例名:fs_in)（顶点着色器中用的是vs_out）可以是随意的
in VS_OUT
{
    vec2 TexCoords;
} fs_in;

