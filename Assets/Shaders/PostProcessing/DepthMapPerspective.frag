#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_DepthMap;

float LinearizeDepth(float depth)
{
    float nearPlane = 1.0;
    float farPlane = 10.0;

    float z = depth * 2.0 - 1.0;
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{   
    float depthValue = texture(u_DepthMap, v_TexCoord).r;
    f_Color = vec4(vec3(LinearizeDepth(depthValue) / 10.0), 1.0);
}