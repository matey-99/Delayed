#version 450 core

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 v_LocalPosition;

layout (location = 0) uniform mat4 u_View;
layout (location = 1) uniform mat4 u_Projection;

void main()
{
    v_LocalPosition = a_Position;

    mat4 rotView = mat4(mat3(u_View));
    vec4 pos = u_Projection * rotView * vec4(v_LocalPosition, 1.0);
    
    gl_Position = pos.xyww;
}