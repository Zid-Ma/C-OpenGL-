#version 330 core

void main()
{             
    //由于没有颜色缓冲，最后的片段不需要任何处理，所以我们可以简单地使用一个空片段着色器：
    // gl_FragDepth = gl_FragCoord.z;//可以取消行的注释，来显式设置深度，但是注释掉是更有效率的，因为底层无论如何都会默认去设置深度缓冲
}
