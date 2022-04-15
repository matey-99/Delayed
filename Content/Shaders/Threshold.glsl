#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;

    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#SHADER FRAGMENT
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