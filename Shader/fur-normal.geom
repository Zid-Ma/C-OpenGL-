#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;//线-条带

in VS_OUT {
    vec3 normal;
} gs_in[];

//量级
const float MAGNITUDE = 0.4;

//几何着色器会接收每一个顶点（包括一个位置向量和一个法向量），并在每个位置向量处绘制一个法线向量
void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;//位置向量位置
    EmitVertex();
    //通过法线向量和位置向量计算出第二个位置
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;//将法向量乘以了一个MAGNITUDE向量，来限制显示出的法向量大小
    EmitVertex();
    EndPrimitive();
}

void main()
{
    //三角形的三个顶点
    GenerateLine(0); // 第一个顶点法线
    GenerateLine(1); // 第二个顶点法线
    GenerateLine(2); // 第三个顶点法线
}