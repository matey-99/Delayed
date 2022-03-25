#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_Input;
layout (location = 1) uniform int u_Size;

void main()
{    
    vec2 texelSize = 1.0 / vec2(textureSize(u_Input, 0));
    vec3 result = vec3(0.0);
    for (int x = -u_Size; x < u_Size; x++)
    {
        for (int y = -u_Size; y < u_Size; y++)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_Input, v_TexCoord + offset).rgb;
        }
    }

    result /= ((u_Size * 2) * (u_Size * 2));
    f_Color = vec4(result, 1.0);
}