#version 450 core

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) out vec4 f_Color;

layout (location = 4) uniform bool u_IsSprite;
layout (location = 5) uniform sampler2D u_Sprite; 
layout (location = 6) uniform vec4 u_Color;

void main()
{
    vec4 color = u_IsSprite ? texture(u_Sprite, v_TexCoord) : u_Color;

    f_Color = color;
}