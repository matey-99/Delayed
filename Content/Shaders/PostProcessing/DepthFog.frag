#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_GBufferPosition;
layout (location = 1) uniform sampler2D u_Screen;

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
    float u_CameraFarClipPlane;
};

void main()
{
    float fog_maxdist = 20.0;
    float fog_mindist = 5.0;
    vec4 fog_colour = vec4(0.4, 0.4, 0.4, 1.0);

    vec4 col = texture(u_Screen, v_TexCoord);
    vec3 pos = vec3(texture(u_GBufferPosition, v_TexCoord));

    // Calculate fog
    float dist = length(pos - u_ViewPosition);
    float fog_factor = (fog_maxdist - dist) / (fog_maxdist - fog_mindist);
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    f_Color = mix(fog_colour, col, fog_factor);
}