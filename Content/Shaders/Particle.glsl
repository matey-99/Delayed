#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_ParticlePosition;
layout (location = 3) in vec4 a_ParticleColor;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec4 v_ParticleColor;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

void main()
{
    vec3 viewRight = vec3(u_View[0][0], u_View[1][0], u_View[2][0]);
    vec3 viewUp = vec3(u_View[0][1], u_View[1][1], u_View[2][1]);
    v_Position = viewRight * a_Position.x * a_ParticlePosition.w + viewUp * a_Position.y * a_ParticlePosition.w + a_ParticlePosition.xyz; 

    v_TexCoord = a_TexCoord;
    v_ParticleColor = a_ParticleColor;

    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec4 v_ParticleColor;

layout (location = 0) out vec4 f_Color;

layout (location = 0) uniform sampler2D u_Sprite;
layout (location = 1) uniform float u_EmissionStrength;

void main()
{
    vec4 sprite = texture(u_Sprite, v_TexCoord);
    sprite = vec4(pow(sprite.rgb, vec3(1.0 / 2.2)), sprite.a);

    f_Color = sprite * v_ParticleColor;
}