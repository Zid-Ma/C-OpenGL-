#version 330 core
//Ϊ�÷�����ͼ�����������ȵ�����ɫ���д���һ��TBN����
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;//��ǰ�������������ߺ͸�������������������ɫ������Ϊ�������ԣ�
layout (location = 4) in vec3 bitangent;
/*��һ�ַ�����ͼ����
out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out; 

//�ڶ�����ɫ����main���������Ǵ���TBN����
void main()
{
   [...]
	//�Ƚ�����TBN�����任������������������ϵ�У�����������ռ䣬���ǿ��Գ���model����Ȼ�����Ǵ���ʵ�ʵ�TBN����ֱ�Ӱ���Ӧ������Ӧ�õ�mat3���������С�ע�⣬�������ϣ������ȷ�Ļ��Ͳ�Ҫ��TBN��������model���󣬶���ʹ�÷��߾��󣬵�����ֻ���������ķ��򣬲���ƽ��Ҳ����������任��
   vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	//�Ӽ����Ͻ���������ɫ�������踱���ߡ����е�������TBN���������໥��ֱ���������ǿ����ڶ�����ɫ������T��N�����Ĳ�ˣ��Լ�����������ߣ�vec3 B = cross(T, N);
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

//�����߿ռ�Ĺ�Դλ�ã��۲�λ���Լ�����λ�÷��͸�������ɫ�����������ǾͲ�����������ɫ������о���˷��ˡ�
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
    //�ڽ�TBN������ʱҪʹ�������
   mat3 TBN = transpose(mat3(T, B, N));

/*����ķ-ʩ�������������̣�Gram-Schmidt process��
//ʹ�ý�������ķ-ʩ�������������̣�Gram-Schmidt process������ѧ���ɣ����ǿ��Զ�TBN����������������������ÿ���������ֻ����´�ֱ�ˡ��ڶ�����ɫ����������������
vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
// re-orthogonalize T with respect to N//����N����������T
T = normalize(T - dot(T, N) * N);
// then retrieve perpendicular vector B with the cross product of T and N//Ȼ����T��N�Ĳ��������ֱ����B
vec3 B = cross(T, N);
mat3 TBN = mat3(T, B, N)
//������΢����һЩ���ܿ������ܶԷ�����ͼ����һ�����������������Ǹ�������Դ�� Normal Mapping Mathematics��Ƶ�������ж�������̵Ľ��͡�
*/
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vec3(model * vec4(position, 0.0));
}