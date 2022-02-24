#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_SourceTexture;
layout (location = 1) uniform vec2 u_TexelOffset;

void main()
{   
    vec2 texCoord = v_TexCoord;
    vec3 col;

    texCoord -= u_TexelOffset * 5.0;
    col = texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += 2.0 * texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += 2.0 * texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += 3.0 * texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += 3.0 * texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += 3.0 * texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += 2.0 * texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += 2.0 * texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += texture2D(u_SourceTexture, texCoord).rgb;

    texCoord += u_TexelOffset;
    col += texture2D(u_SourceTexture, texCoord).rgb;

    col /= 21.0;

    f_Color = vec4(col, 1.0);
}