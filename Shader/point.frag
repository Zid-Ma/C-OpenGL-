#version 330 core
layout (depth_<condition>) out float gl_FragDepth;//��Ƭ����ɫ���Ķ���ʹ���������(Depth Condition)��������gl_FragDepth������
/*condition����Ϊ�����ֵ��

����	����
any	Ĭ��ֵ����ǰ��Ȳ����ǽ��õģ������ʧ�ܶ�����
greater	��ֻ�������ֵ��gl_FragCoord.z����
less	��ֻ�������ֵ��gl_FragCoord.z��С
unchanged	�����Ҫд��gl_FragDepth���㽫ֻ��д��gl_FragCoord.z��ֵ
*/

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{             
    //gl_FrontFacing������һ��bool�������ǰƬ�����������һ������ô����true���������false
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoords);
    else
        FragColor = texture(backTexture, TexCoords);

    //����ɫ��������Ƭ�ε����ֵ��
    gl_FragDepth = 0.0; // ���Ƭ�����ڵ����ֵΪ 0.0//�����ɫ��û��д��ֵ��gl_FragDepth�������Զ�ȡ��gl_FragCoord.z��ֵ��
    //ֻҪ������Ƭ����ɫ���ж�gl_FragDepth����д�룬OpenGL�ͻᣨ����Ȳ���С�������۵��������������е���ǰ��Ȳ���(Early Depth Testing)��
    //�������õ�ԭ���ǣ�OpenGL�޷���Ƭ����ɫ������֮ǰ��֪Ƭ�ν�ӵ�е����ֵ����ΪƬ����ɫ�����ܻ���ȫ�޸�������ֵ��
}

/*
//��Ƭ�ε����ֵ�����˵���������ȻҲ������һЩ��ǰ��Ȳ��ԣ�

#version 420 core // ע��GLSL�İ汾��
out vec4 FragColor;
layout (depth_greater) out float gl_FragDepth;

void main()
{             
    FragColor = vec4(1.0);
    gl_FragDepth = gl_FragCoord.z + 0.1;
}
*/

//����һ������ӿڿ顣����(Block Name)Ӧ���Ǻ���ɫ����һ���ģ�VS_OUT������ʵ����(Instance Name)(ʵ����:fs_in)��������ɫ�����õ���vs_out�������������
in VS_OUT
{
    vec2 TexCoords;
} fs_in;

