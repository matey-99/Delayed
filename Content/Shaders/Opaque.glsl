#SHADER VERTEX
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
layout (location = 3) out vec3 v_Tangent;
layout (location = 4) out vec3 v_Bitangent;
layout (location = 5) out vec4 v_ViewPosition;
layout (location = 6) out vec4[MAX_SPOT_LIGHTS] v_SpotLightSpacePositions;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

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
layout (location = 1) uniform bool u_IsSkeletalMesh;

uniform mat4 u_FinalBonesMatrices[MAX_BONES];

void CalculatePositionWithBoneInfluence(out vec3 position, out vec3 normal)
{
    vec4 totalPosition = vec4(0.0);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(a_BoneIds[i] == -1) 
            continue;
        if(a_BoneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(a_Position, 1.0);
            break;
        }
        vec4 localPosition = u_FinalBonesMatrices[a_BoneIds[i]] * vec4(a_Position, 1.0);
        totalPosition += localPosition * a_Weights[i];
        normal = mat3(u_FinalBonesMatrices[a_BoneIds[i]]) * a_Normal;
    }

    position = vec3(totalPosition);
}

void main()
{
    vec3 position = vec3(0.0);
    vec3 normal = vec3(0.0);
    if (u_IsSkeletalMesh)
        CalculatePositionWithBoneInfluence(position, normal);
    else
    {
        position = a_Position;
        normal = a_Normal;
    }

    v_Position = vec3(a_Model * vec4(position, 1.0));
    v_ViewPosition = u_View * vec4(v_Position, 1.0);
    v_Normal = mat3(transpose(inverse(a_Model))) * normal;

    if (u_MaterialVS.flipVerticallyUV)
    {
        vec2 texCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
        v_TexCoord = texCoord;
    }
    else
    {
        v_TexCoord = a_TexCoord;
    }

    v_Tangent = a_Tangent;
    v_Bitangent = a_Bitangent;

    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
        v_SpotLightSpacePositions[i] = u_SpotLightSpaceMatrices[i] * vec4(v_Position, 1.0);

    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Position;
layout (location = 1) out vec4 f_Normal;
layout (location = 2) out vec4 f_ColorAO;
layout (location = 3) out vec4 f_Emissive;
layout (location = 4) out vec4 f_MetallicRoughness;
layout (location = 5) out vec4 f_ViewSpacePosition;
layout (location = 6) out float f_Depth;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec3 v_Tangent;
layout (location = 4) in vec3 v_Bitangent;
layout (location = 5) in vec4 v_ViewPosition;

struct Material
{
    bool isAlbedoMap;
    bool isNormalMap;
    bool isMetallicMap;
    bool isRoughnessMap;
    bool isAOMap;
    bool isOpacityMap;
    bool isEmissiveMap;

    sampler2D albedoMap;
    sampler2D metallicMap;
    sampler2D normalMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D opacityMap;
    sampler2D emissiveMap;

    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    float opacity;
    vec3 emissive;

    float emissiveStrength;
};

layout (location = 2) uniform Material u_Material;

vec3 GetNormalFromNormalMap()
{
    mat3 TBN = mat3(v_Tangent, v_Bitangent, normalize(v_Normal));
    return normalize(TBN * (texture(u_Material.normalMap, v_TexCoord).rgb * 2.0 - 1.0));
}

void main()
{
    vec3 albedo;
    vec3 N;
    float metallic;
    float roughness;
    float ao;
    float opacity;
    vec3 emissive;
    if (u_Material.isAlbedoMap)
        albedo = pow(texture(u_Material.albedoMap, v_TexCoord).rgb, vec3(1 / 2.2));
    else
        albedo = u_Material.albedo;
    
    if (u_Material.isNormalMap)
        N = GetNormalFromNormalMap();
    else
        N = normalize(v_Normal);
    
    if (u_Material.isMetallicMap)
        metallic = texture(u_Material.metallicMap, v_TexCoord).r;
    else
        metallic = u_Material.metallic;

    if (u_Material.isRoughnessMap)
        roughness = texture(u_Material.roughnessMap, v_TexCoord).r;
    else
        roughness = u_Material.roughness;

    if (u_Material.isAOMap)
        ao = texture(u_Material.aoMap, v_TexCoord).r;
    else
        ao = u_Material.ao;

    if (u_Material.isOpacityMap)
        opacity = texture(u_Material.opacityMap, v_TexCoord).r;
    else
        opacity = u_Material.opacity;

    if (u_Material.isEmissiveMap)
        emissive = texture(u_Material.emissiveMap, v_TexCoord).rgb;
    else
        emissive = u_Material.emissive;

    if (opacity < 0.1)
        discard;

    // OUTPUT
    f_Position = vec4(v_Position, 1.0);

    f_Normal = vec4(N, 1.0);

    f_ColorAO.rgb = albedo;
    f_ColorAO.a = ao;

    f_Emissive = vec4(emissive * u_Material.emissiveStrength, 1.0);

    f_MetallicRoughness = vec4(metallic, roughness, 0.0, 1.0);

    f_ViewSpacePosition = v_ViewPosition;

    f_Depth = gl_FragCoord.z;
}