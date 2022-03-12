#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_Screen;
layout (location = 1) uniform float u_Threshold;
layout (location = 2) uniform float u_Limit;

void main()
{    
    vec3 color = texture(u_Screen, v_TexCoord).rgb;

    f_Color = vec4(clamp(color - u_Threshold, 0, u_Limit), 1.0);
}