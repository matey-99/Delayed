#version 450 core

layout (location = 0) in vec3 a_Position;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

layout (location = 0) uniform mat4 u_Model;

void main()
{
    vec3 position = vec3(u_Model * vec4(a_Position, 1.0));
    gl_Position = u_ViewProjection * vec4(position, 1.0);
}