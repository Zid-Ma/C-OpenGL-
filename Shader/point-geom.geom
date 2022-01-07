#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{    
    //绘制出一个房子的顶点
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex//gs_in[0]因为只有一个输入顶点        //输入点为房子中间
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   // 1:左下
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right  //2:右下
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left      //3:左上
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right     // 4:右上
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top           // 5:顶部
    fColor = vec3(1.0, 1.0, 1.0);//仅仅是为了有趣，我们也可以假装这是冬天，将最后一个顶点的颜色设置为白色，给屋顶落上一些雪
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}

//几何着色器
//----------
//声明从顶点着色器输入的图元类型   //需要在in关键字前声明一个布局修饰符(Layout Qualifier)
/*
//这个输入布局修饰符可以从顶点着色器接收下列任何一个图元值：
(括号内的数字表示的是一个图元所包含的最小顶点数)

points：                 绘制GL_POINTS图元时（1）。
lines：                  绘制GL_LINES或GL_LINE_STRIP时（2）
lines_adjacency：        GL_LINES_ADJACENCY或GL_LINE_STRIP_ADJACENCY（4）
triangles：              GL_TRIANGLES、GL_TRIANGLE_STRIP或GL_TRIANGLE_FAN（3）
triangles_adjacency：    GL_TRIANGLES_ADJACENCY或GL_TRIANGLE_STRIP_ADJACENCY（6）
*/
//layout (points) in;
/*
输出布局修饰符也可以接受几个图元值：
(有了这3个输出修饰符，我们就可以使用输入图元创建几乎任意的形状了。要生成一个三角形的话，我们将输出定义为triangle_strip，并输出3个顶点)

points
line_strip      //线条:连接了一组点，形成一条连续的线，它最少要由两个点来组成
triangle_strip

几何着色器同时希望我们设置一个它最大能够输出的顶点数量（如果你超过了这个值，OpenGL将不会绘制多出的顶点），
这个也可以在out关键字的布局修饰符中设置。在这个例子中，我们将输出一个line_strip，并将最大顶点数设置为2个。
*/
//指定几何着色器输出的图元类型
//layout (line_strip, max_vertices = 2) out;
/*
//GLSL提供给我们一个内建(Built-in)变量
//这里，它被声明为一个接口块             //它被声明为一个数组，因为大多数的渲染图元包含多于1个的顶点，而几何着色器的输入是一个图元的所有顶点。
in gl_Vertex
{
    vec4  gl_Position;      //和顶点着色器输出非常相似的一个向量
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];
*/
/*
//在这个例子中，我们发射了两个顶点，它们从原始顶点位置平移了一段距离，之后调用了EndPrimitive，将这两个顶点合成为一个包含两个顶点的线条。
void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    //每次调用时，gl_Position中的向量会被添加到图元中来
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    //被调用时，所有发射出的(Emitted)顶点都会合成为指定的输出渲染图元    //在一个或多个EmitVertex调用之后重复调用EndPrimitive能够生成多个图元
    EndPrimitive();
}
*/
/*
//创建一个传递(Pass-through)几何着色器，它会接收一个点图元，并直接将它传递(Pass)到下一个着色器：
//---------------------------------------------------------------------------------------------
layout (points) in;
layout (points, max_vertices = 1) out;

void main() {    
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    EndPrimitive();
}
*/
