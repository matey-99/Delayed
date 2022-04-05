#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) uniform vec3 u_Color;

void main()
{
    f_Color = vec4(u_Color, 1.0);
}