#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_Screen;
layout (location = 1) uniform float u_Threshold;

void main()
{    
    vec3 col = texture(u_Screen, v_TexCoord).rgb;
    float brightness = dot(col.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness <= u_Threshold)
        col = vec3(0.0);

    f_Color = vec4(col, 1.0);
}