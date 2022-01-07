#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    //��õ�ǰƬ����ɫ
    vec4 texColor = texture(texture1, TexCoords);
    if(texColor.a < 0.1)
        //���ᱣ֤Ƭ�β��ᱻ��һ���������ԾͲ��������ɫ���塣
        //���ܹ���Ƭ����ɫ���м��һ��Ƭ�ε�alphaֵ�Ƿ����ĳ����ֵ������ǵĻ����������Ƭ�Σ��ͺ�����������һ��
        discard;
    FragColor = texColor;
}