#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;

layout (location = 1) uniform sampler2D u_Image;
layout (location = 2) uniform vec4 u_Color;

void main()
{    
    vec4 image = texture(u_Image, v_TexCoord);
    image *= u_Color;

    f_Color = image;
}