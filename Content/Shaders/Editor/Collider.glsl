#SHADER VERTEX
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

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) uniform vec3 u_Color;

void main()
{
    f_Color = vec4(u_Color, 1.0);
}