#version 450 core

in vec4 g_Position;

layout (location = 7) uniform vec3 u_LightPos;
layout (location = 8) uniform float u_FarPlane;

void main()
{   
    float lightDistance = length(g_Position.xyz - u_LightPos);
    lightDistance = lightDistance / u_FarPlane;
    gl_FragDepth = lightDistance;
}