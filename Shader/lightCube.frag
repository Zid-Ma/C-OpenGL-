#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set alle 4 vector values to 1.0


    /*
    //���Ƭ������Ƿ������ֵ�����������ֵ�������Ϊ������ɫ Check whether fragment output is higher than threshold, if so output as brightness color
    //�Ȼ�ȡ��ǰƬ�ε�����
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);

    //����
    //-----
    //��˹ģ��(Gaussian blur)��
    */
}

