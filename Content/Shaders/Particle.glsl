#SHADER VERTEX
#version 450 core

layout (location = 0) out vec2 v_TexCoord;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
};

layout (std140, binding = 1) buffer Position
{
    vec4 positions[];
};

layout (location = 0) uniform mat4 u_Model;
layout (location = 1) uniform mat4 u_View;
layout (location = 2) uniform mat4 u_Projection;
layout (location = 3) uniform vec2 u_SpriteSize;

void main()
{
    int particleID = gl_VertexID >> 2;
    vec4 particlePos = positions[particleID];

    /*  
        gl_VertexID = 0: x = 1.0, y = 0.0
        gl_VertexID = 1: x = 0.0, y = 0.0
        gl_VertexID = 2: x = 0.0, y = 1.0
        gl_VertexID = 3: x = 1.0, y = 1.0
     */
    vec2 quadPos = vec2(((gl_VertexID - 1) & 2) >> 1, (gl_VertexID & 2) >> 1);

    vec4 particlePosEye = u_View * u_Model * particlePos;
    vec4 vertexPosEye = particlePosEye + vec4((quadPos * 2.0 - 1.0) * u_SpriteSize, 0, 0);

    v_TexCoord = quadPos;
    gl_Position = u_Projection * vertexPosEye;
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) out vec4 f_Color;

layout (location = 4) uniform bool u_IsSprite;
layout (location = 5) uniform sampler2D u_Sprite; 
layout (location = 6) uniform vec4 u_Color;

void main()
{
    vec4 color = u_IsSprite ? texture(u_Sprite, v_TexCoord) : u_Color;

    f_Color = color;
}