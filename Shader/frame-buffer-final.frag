#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//泛光操作-最终颜色合成
uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;
uniform float exposure = 1.0;//曝光值
void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0f);
}