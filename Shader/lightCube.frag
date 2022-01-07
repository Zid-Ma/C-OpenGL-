#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set alle 4 vector values to 1.0


    /*
    //检查片段输出是否高于阈值，如果高于阈值，则输出为亮度颜色 Check whether fragment output is higher than threshold, if so output as brightness color
    //先获取当前片段的亮度
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);

    //泛光
    //-----
    //高斯模糊(Gaussian blur)。
    */
}

