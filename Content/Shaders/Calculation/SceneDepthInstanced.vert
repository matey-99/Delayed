#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 4) in mat4 a_InstancedMatrix;

layout (location = 0) uniform mat4 u_LightSpace;

void main()
{
    gl_Position = u_LightSpace * a_InstancedMatrix * vec4(a_Position, 1.0);
}