#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 4) in mat4 a_InstancedMatrix;

void main()
{
    gl_Position = a_InstancedMatrix * vec4(a_Position, 1.0);
}