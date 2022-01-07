#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{    
    //���Ƴ�һ�����ӵĶ���
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex//gs_in[0]��Ϊֻ��һ�����붥��        //�����Ϊ�����м�
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   // 1:����
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right  //2:����
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left      //3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right     // 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top           // 5:����
    fColor = vec3(1.0, 1.0, 1.0);//������Ϊ����Ȥ������Ҳ���Լ�װ���Ƕ��죬�����һ���������ɫ����Ϊ��ɫ�����ݶ�����һЩѩ
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}

//������ɫ��
//----------
//�����Ӷ�����ɫ�������ͼԪ����   //��Ҫ��in�ؼ���ǰ����һ���������η�(Layout Qualifier)
/*
//������벼�����η����ԴӶ�����ɫ�����������κ�һ��ͼԪֵ��
(�����ڵ����ֱ�ʾ����һ��ͼԪ����������С������)

points��                 ����GL_POINTSͼԪʱ��1����
lines��                  ����GL_LINES��GL_LINE_STRIPʱ��2��
lines_adjacency��        GL_LINES_ADJACENCY��GL_LINE_STRIP_ADJACENCY��4��
triangles��              GL_TRIANGLES��GL_TRIANGLE_STRIP��GL_TRIANGLE_FAN��3��
triangles_adjacency��    GL_TRIANGLES_ADJACENCY��GL_TRIANGLE_STRIP_ADJACENCY��6��
*/
//layout (points) in;
/*
����������η�Ҳ���Խ��ܼ���ͼԪֵ��
(������3��������η������ǾͿ���ʹ������ͼԪ���������������״�ˡ�Ҫ����һ�������εĻ������ǽ��������Ϊtriangle_strip�������3������)

points
line_strip      //����:������һ��㣬�γ�һ���������ߣ�������Ҫ�������������
triangle_strip

������ɫ��ͬʱϣ����������һ��������ܹ�����Ķ�������������㳬�������ֵ��OpenGL��������ƶ���Ķ��㣩��
���Ҳ������out�ؼ��ֵĲ������η������á�����������У����ǽ����һ��line_strip��������󶥵�������Ϊ2����
*/
//ָ��������ɫ�������ͼԪ����
//layout (line_strip, max_vertices = 2) out;
/*
//GLSL�ṩ������һ���ڽ�(Built-in)����
//�����������Ϊһ���ӿڿ�             //��������Ϊһ�����飬��Ϊ���������ȾͼԪ��������1���Ķ��㣬��������ɫ����������һ��ͼԪ�����ж��㡣
in gl_Vertex
{
    vec4  gl_Position;      //�Ͷ�����ɫ������ǳ����Ƶ�һ������
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];
*/
/*
//����������У����Ƿ������������㣬���Ǵ�ԭʼ����λ��ƽ����һ�ξ��룬֮�������EndPrimitive��������������ϳ�Ϊһ���������������������
void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    //ÿ�ε���ʱ��gl_Position�е������ᱻ��ӵ�ͼԪ����
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    //������ʱ�����з������(Emitted)���㶼��ϳ�Ϊָ���������ȾͼԪ    //��һ������EmitVertex����֮���ظ�����EndPrimitive�ܹ����ɶ��ͼԪ
    EndPrimitive();
}
*/
/*
//����һ������(Pass-through)������ɫ�����������һ����ͼԪ����ֱ�ӽ�������(Pass)����һ����ɫ����
//---------------------------------------------------------------------------------------------
layout (points) in;
layout (points, max_vertices = 1) out;

void main() {    
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    EndPrimitive();
}
*/
