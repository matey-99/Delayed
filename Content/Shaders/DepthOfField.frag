#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_GBufferViewPosition;
layout (location = 1) uniform sampler2D u_Input;
layout (location = 2) uniform sampler2D u_BlurredInput;
layout (location = 3) uniform float u_MinDistance;
layout (location = 4) uniform float u_MaxDistance;

void main()
{   
    vec3 position = texture(u_GBufferViewPosition, v_TexCoord).rgb; 
    vec3 focus = texture(u_Input, v_TexCoord).rgb;
    vec3 outOfFocus = texture(u_BlurredInput, v_TexCoord).rgb;

    vec3 focusPoint = texture(u_GBufferViewPosition, textureSize(u_GBufferViewPosition, 0) / 2).rgb;

    float blur = smoothstep(u_MinDistance, u_MaxDistance, length(position - focusPoint));

    f_Color = vec4(mix(focus, outOfFocus, blur), 1.0);
}