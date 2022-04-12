#version 450 core

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec4 v_ViewPosition;
layout (location = 4) in vec4[MAX_SPOT_LIGHTS] v_SpotLightSpacePositions;

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

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct PointLight
{
    vec3 position;
    vec3 color;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float innerCutOff;
    float outerCutOff;
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

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
    float u_CameraFarClipPlane;
};

layout (std140, binding = 3) uniform u_FragmentLights
{
    int u_PointLightsCount;
    int u_SpotLightsCount;

    DirectionalLight u_DirectionalLight;
    PointLight[MAX_POINT_LIGHTS] u_PointLights;
    SpotLight[MAX_SPOT_LIGHTS] u_SpotLights;
};

layout (location = 2) uniform Material u_Material;
layout (location = 23) uniform sampler2DArray u_DirectionalLightShadowMaps;
layout (location = 24) uniform vec3 u_SkyLightColor;
layout (location = 25) uniform float u_SkyLightIntensity;

const float PI = 3.14159265359;

vec3 GetNormalFromNormalMap()
{
    vec3 tangentNormal = texture(u_Material.normalMap, v_TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(v_Position);
    vec3 Q2 = dFdy(v_Position);
    vec2 st1 = dFdx(v_TexCoord);
    vec2 st2 = dFdy(v_TexCoord);

    vec3 N = normalize(v_Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 CalculateLight(vec3 L, vec3 V, vec3 albedo, vec3 N, float metallic, float roughness)
{
    vec3 H = normalize(V + L);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * NdotL;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 V, vec3 albedo, vec3 N, float metallic, float roughness)
{
    vec3 L = normalize(-light.direction);

    return CalculateLight(L, V, albedo, N, metallic, roughness) * light.color;
}

vec3 CalculatePointLight(PointLight light, vec3 V, vec3 albedo, vec3 N, float metallic, float roughness)
{
    vec3 L = normalize(light.position - v_Position);

    float dist = length(light.position - v_Position);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = light.color * attenuation;

    return CalculateLight(L, V, albedo, N, metallic, roughness) * radiance;
}

vec3 CalculateSpotLight(SpotLight light, vec3 V, vec3 albedo, vec3 N, float metallic, float roughness)
{
    vec3 L = normalize(light.position - v_Position);

    float dist = length(light.position - v_Position);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = light.color * attenuation;

    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return CalculateLight(L, V, albedo, N, metallic, roughness) * intensity * radiance;
}

// TO DO: FIX
// float CalculateDirectionalLightShadow(vec3 position, vec3 normal)
// {
//     vec4 viewSpace = u_View * vec4(position, 1.0);
//     float depthValue = abs(viewSpace.z);

//     int layer = -1;
//     for (int i = 0; i < u_CascadeCount; i++)
//     {
//         if (depthValue < u_CascadeClipPlaneDistances[i])
//         {
//             layer = i;
//             break;
//         }
//     }
//     if (layer == -1)
//             layer = u_CascadeCount;
    
//     vec4 lightSpace = u_DirectionalLightSpaceMatrices[layer] * vec4(position, 1.0);
//     vec3 projectionCoords = lightSpace.xyz / lightSpace.w;
//     projectionCoords = projectionCoords * 0.5 + 0.5;

//     float currentDepth = projectionCoords.z;

//     if (currentDepth > 1.0)
//         return 0.0;

//     float bias = max(0.05 * (1.0 - dot(normal, u_DirectionalLight.direction)), 0.005);
//     float biasModifier = 0.5;
//     if (layer == u_CascadeCount)
//         bias *= 1 / (u_CameraFarClipPlane * biasModifier);
//     else
//         bias *= 1 / (u_CascadeClipPlaneDistances[layer] * biasModifier);

//     // PCF
//     float shadow = 0.0;
//     vec2 texelSize = 1.0 / vec2(textureSize(u_DirectionalLightShadowMaps, 0));
//     for (int x = -1; x <= 1; ++x)
//     {
//         for (int y = -1; y <= 1; ++y)
//         {
//             float pcfDepth = texture(u_DirectionalLightShadowMaps, vec3(projectionCoords.xy + vec2(x, y) * texelSize, layer)).r;
//             shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
//         }
//     }

//     shadow /= 9.0;

//     if (projectionCoords.z > 1.0)
//         shadow = 0.0;

//     return shadow;
// }

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
        albedo = pow(texture(u_Material.albedoMap, v_TexCoord).rgb, vec3(2.2));
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
        emissive = texture(u_Material.emissiveMap, v_TexCoord).rgb * u_Material.emissiveStrength;
    else
        emissive = u_Material.emissive * u_Material.emissiveStrength;

    if (opacity < 0.01)
        discard;

    vec3 V = normalize(u_ViewPosition - v_Position);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // Directional Light
    float shadow = 0.0;
    // shadow = CalculateDirectionalLightShadow(v_Position, N);
    // shadow = clamp(shadow, 0.0, 1.0);

    Lo += CalculateDirectionalLight(u_DirectionalLight, V, albedo, N, metallic, roughness);
    Lo *= (1 - shadow);

    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        Lo += CalculatePointLight(u_PointLights[i], V, albedo, N, metallic, roughness);
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
        Lo += CalculateSpotLight(u_SpotLights[i], V, albedo, N, metallic, roughness);

    vec3 ambient = u_SkyLightColor * u_SkyLightIntensity * albedo * ao;

    vec3 color = ambient + Lo + emissive;
    f_Color = vec4(color, opacity);
}