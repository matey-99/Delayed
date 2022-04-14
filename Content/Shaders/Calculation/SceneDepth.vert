#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 5) in mat4 a_Model;

void main()
{
    gl_Position = a_Model * vec4(a_Position, 1.0);
}