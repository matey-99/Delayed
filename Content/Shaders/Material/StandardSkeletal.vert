#version 450 core

#define MAX_SPOT_LIGHTS 16

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;
layout (location = 5) in mat4 a_Model;
layout (location = 9) in ivec4 a_BoneIds;
layout (location = 10) in vec4 a_Weights;


layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoord;
layout (location = 3) out vec4 v_ViewPosition;
layout (location = 4) out vec4[MAX_SPOT_LIGHTS] v_SpotLightSpacePositions;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

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

layout (location = 0) uniform Material u_MaterialVS;

void main()
{
    // Recalculate position with bones influence
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(a_BoneIds[i] == -1) 
            continue;
        if(a_BoneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(a_Position, 1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[a_BoneIds[i]] * vec4(a_Position, 1.0f);
        totalPosition += localPosition * a_Weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[a_BoneIds[i]]) * a_Normal;
    }

    v_Position = vec3(a_Model * totalPosition);
    v_ViewPosition = u_View * vec4(v_Position, 1.0);
    v_Normal = mat3(transpose(inverse(a_Model))) * a_Normal;

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