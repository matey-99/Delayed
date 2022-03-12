#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_SourceTexture;

void main()
{   
    vec3 color = texture(u_SourceTexture, v_TexCoord).rgb;

    f_Color = vec4(color, 1);
}