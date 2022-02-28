#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_DepthMap;

void main()
{    
    float depthValue = texture(u_DepthMap, v_TexCoord).r;
    f_Color = vec4(vec3(depthValue), 1.0);
}