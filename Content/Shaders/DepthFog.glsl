#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;

    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_GBufferPosition;
layout (location = 1) uniform sampler2D u_Screen;

layout (location = 2) uniform float u_FogMinimalDistance;
layout (location = 3) uniform float u_FogMaximumDistance;
layout (location = 4) uniform float u_Density;
layout (location = 5) uniform vec3 u_FogColor;

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
    float u_CameraFarClipPlane;
};

void main()
{
    vec4 color = texture(u_Screen, v_TexCoord);
    vec4 fragmentPosition = texture(u_GBufferPosition, v_TexCoord);

    // Calculate fog
    float dist = length(vec3(fragmentPosition) - u_ViewPosition);
    float fogFactor = (u_FogMaximumDistance - dist) / (u_FogMaximumDistance - u_FogMinimalDistance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    f_Color = mix(vec4(u_FogColor, 1.0), color, fogFactor);
}