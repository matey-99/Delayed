#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_SourceTexture;
layout (location = 1) uniform vec2 u_TexelOffset;

void main()
{    
    vec3 col;
    col  = texture2D(u_SourceTexture, v_TexCoord + vec2(u_TexelOffset)).rgb;
    col += texture2D(u_SourceTexture, v_TexCoord + vec2(u_TexelOffset.x, -u_TexelOffset.y)).rgb;
    col += texture2D(u_SourceTexture, v_TexCoord + vec2(-u_TexelOffset.x, u_TexelOffset.y)).rgb;
    col += texture2D(u_SourceTexture, v_TexCoord + vec2(-u_TexelOffset)).rgb;
    col *= 0.25;

    f_Color = vec4(col, 1.0);
}