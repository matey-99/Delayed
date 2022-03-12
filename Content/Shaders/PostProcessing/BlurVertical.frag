#version 450 core

#define GAUSSIAN_BLUR_RADIUS 6
#define GAUSSIAN_BLUR_KERNEL_SIZE (GAUSSIAN_BLUR_RADIUS * 2 + 1)

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_SourceTexture;
layout (location = 1) uniform vec2 u_GaussianBlurCache[GAUSSIAN_BLUR_KERNEL_SIZE];

void main()
{   
    vec3 color;
    for (int i = 0; i < GAUSSIAN_BLUR_KERNEL_SIZE; i++)
    {
        color += texture(u_SourceTexture, v_TexCoord + vec2(0.0, u_GaussianBlurCache[i].y)).rgb * u_GaussianBlurCache[i].x;
    }

    f_Color = vec4(color, 1.0);
}