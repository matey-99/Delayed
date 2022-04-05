#version 450 core

layout (location = 0) in vec3 a_Position;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

void main()
{
    vec3 position = a_Position;
    gl_Position = u_ViewProjection * vec4(position, 1.0);
}