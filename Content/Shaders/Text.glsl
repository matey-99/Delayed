#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

layout (location = 0) uniform mat4 u_Model;

void main()
{
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_TexCoord = a_TexCoord;

    gl_Position = vec4(v_Position, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;

layout (location = 1) uniform sampler2D u_Text;
layout (location = 2) uniform vec4 u_Color;

void main()
{    
    float text = texture(u_Text, v_TexCoord).r;
    f_Color = vec4(u_Color.rgb, u_Color.a * text);
}