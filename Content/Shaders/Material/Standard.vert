#version 450 core

#define MAX_SPOT_LIGHTS 16

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoord;
layout (location = 3) out vec4 v_ViewPosition;
layout (location = 4) out vec4[MAX_SPOT_LIGHTS] v_SpotLightSpacePositions;

struct Material
{
    bool flipVerticallyUV;
};

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

layout (std140, binding = 1) uniform u_VertexLights
{
    // Directional Light
    mat4 u_DirectionalLightSpaceMatrices[16];
    int u_CascadeCount;
    float u_CascadeClipPlaneDistances[4];

    // Spot Light
    mat4[MAX_SPOT_LIGHTS] u_SpotLightSpaceMatrices;
};

layout (location = 0) uniform mat4 u_Model;
layout (location = 1) uniform Material u_MaterialVS;

void main()
{
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_ViewPosition = u_View * vec4(v_Position, 1.0);
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    if (u_MaterialVS.flipVerticallyUV)
    {
        vec2 texCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
        v_TexCoord = texCoord;
    }
    else
    {
        v_TexCoord = a_TexCoord;
    }

    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
        v_SpotLightSpacePositions[i] = u_SpotLightSpaceMatrices[i] * vec4(v_Position, 1.0);

    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}