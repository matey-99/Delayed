#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 v_LocalPosition;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

void main()
{
    v_LocalPosition = a_Position;

    mat4 rotView = mat4(mat3(u_View));
    vec4 pos = u_Projection * rotView * vec4(v_LocalPosition, 1.0);
    
    gl_Position = pos.xyww;
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_LocalPosition;

layout (location = 2) uniform samplerCube u_EnvironmentMap;

void main()
{    
    vec3 envColor = textureLod(u_EnvironmentMap, v_LocalPosition, 1.2).rgb;

    f_Color = vec4(envColor, 1.0);
}