#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_Screen;
layout (location = 1) uniform sampler2D u_Gizmos;

void main()
{    
    vec3 col = texture(u_Screen, v_TexCoord).rgb;
    col += texture(u_Gizmos, v_TexCoord).rgb;

    f_Color = vec4(col, 1.0);
}