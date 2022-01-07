#version 330 core
//为让法线贴图工作，我们先得在着色器中创建一个TBN矩阵。
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;//将前面计算出来的切线和副切线向量传给顶点着色器，作为它的属性：
layout (location = 4) in vec3 bitangent;
/*第一种法线贴图方法
out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out; 

//在顶点着色器的main函数中我们创建TBN矩阵：
void main()
{
   [...]
	//先将所有TBN向量变换到我们所操作的坐标系中，现在是世界空间，我们可以乘以model矩阵。然后我们创建实际的TBN矩阵，直接把相应的向量应用到mat3构造器就行。注意，如果我们希望更精确的话就不要将TBN向量乘以model矩阵，而是使用法线矩阵，但我们只关心向量的方向，不会平移也和缩放这个变换。
   vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	//从技术上讲，顶点着色器中无需副切线。所有的这三个TBN向量都是相互垂直的所以我们可以在顶点着色器中用T和N向量的叉乘，自己计算出副切线：vec3 B = cross(T, N);
   vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
   vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
   //mat3 TBN = mat3(T, B, N);
    vs_out.TBN = mat3(T, B, N);
}
*/

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

//将切线空间的光源位置，观察位置以及顶点位置发送给像素着色器。这样我们就不用在像素着色器里进行矩阵乘法了。
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));   
    vs_out.TexCoords = texCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);    
    //在将TBN矩阵发送时要使用逆矩阵
   mat3 TBN = transpose(mat3(T, B, N));

/*格拉姆-施密特正交化过程（Gram-Schmidt process）
//使用叫做格拉姆-施密特正交化过程（Gram-Schmidt process）的数学技巧，我们可以对TBN向量进行重正交化，这样每个向量就又会重新垂直了。在顶点着色器中我们这样做：
vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
// re-orthogonalize T with respect to N//关于N重新正交化T
T = normalize(T - dot(T, N) * N);
// then retrieve perpendicular vector B with the cross product of T and N//然后用T和N的叉积检索垂直向量B
vec3 B = cross(T, N);
mat3 TBN = mat3(T, B, N)
//这样稍微花费一些性能开销就能对法线贴图进行一点提升。看看最后的那个附加资源： Normal Mapping Mathematics视频，里面有对这个过程的解释。
*/
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vec3(model * vec4(position, 0.0));
}