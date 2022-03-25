#version 450 core

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_GBufferPosition;
layout (location = 1) uniform sampler2D u_GBufferNormal;
layout (location = 2) uniform sampler2D u_GBufferColorAO;
layout (location = 3) uniform sampler2D u_GBufferEmissive;
layout (location = 4) uniform sampler2D u_GBufferMetallicRoughness;
layout (location = 5) uniform sampler2DArray u_DirectionalLightShadowMaps;
layout (location = 6) uniform sampler2D u_SSAO;

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

const float PI = 3.14159265359;

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

vec3 CalculatePointLight(PointLight light, vec3 position, vec3 V, vec3 albedo, vec3 N, float metallic, float roughness)
{
    if (light.color == vec3(0.0))
        return vec3(0.0);

    vec3 L = normalize(light.position - position);

    float dist = length(light.position - position);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = light.color * attenuation;

    return CalculateLight(L, V, albedo, N, metallic, roughness) * radiance;
}

vec3 CalculateSpotLight(SpotLight light, vec3 position, vec3 V, vec3 albedo, vec3 N, float metallic, float roughness)
{
    if (light.color == vec3(0.0))
        return vec3(0.0);

    vec3 L = normalize(light.position - position);

    float dist = length(light.position - position);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = light.color * attenuation;

    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return CalculateLight(L, V, albedo, N, metallic, roughness) * intensity * radiance;
}

float CalculateDirectionalLightShadow(vec3 position, vec3 normal)
{
    vec4 viewSpace = u_View * vec4(position, 1.0);
    float depthValue = abs(viewSpace.z);

    int layer = -1;
    for (int i = 0; i < u_CascadeCount; i++)
    {
        if (depthValue < u_CascadeClipPlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
            layer = u_CascadeCount;
    
    vec4 lightSpace = u_DirectionalLightSpaceMatrices[layer] * vec4(position, 1.0);
    vec3 projectionCoords = lightSpace.xyz / lightSpace.w;
    projectionCoords = projectionCoords * 0.5 + 0.5;

    float currentDepth = projectionCoords.z;

    if (currentDepth > 1.0)
        return 0.0;

    float bias = max(0.05 * (1.0 - dot(normal, u_DirectionalLight.direction)), 0.005);
    float biasModifier = 0.5;
    if (layer == u_CascadeCount)
        bias *= 1 / (u_CameraFarClipPlane * biasModifier);
    else
        bias *= 1 / (u_CascadeClipPlaneDistances[layer] * biasModifier);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(u_DirectionalLightShadowMaps, 0));
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_DirectionalLightShadowMaps, vec3(projectionCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if (projectionCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{    
    vec3 position = texture(u_GBufferPosition, v_TexCoord).rgb;
    vec3 normal = texture(u_GBufferNormal, v_TexCoord).rgb;
    vec3 color = texture(u_GBufferColorAO, v_TexCoord).rgb;
    float ao = texture(u_GBufferColorAO, v_TexCoord).a * texture(u_SSAO, v_TexCoord).r; // EXPERIMENTAL
    vec3 emissive = texture(u_GBufferEmissive, v_TexCoord).rgb;
    float metallic = texture(u_GBufferMetallicRoughness, v_TexCoord).r;
    float roughness = texture(u_GBufferMetallicRoughness, v_TexCoord).g;

    vec3 V = normalize(u_ViewPosition - position);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, color, metallic);

    vec3 Lo = vec3(0.0);

    // Directional Light
    float shadow = 0.0;
    shadow = CalculateDirectionalLightShadow(position, normal);
    shadow = clamp(shadow, 0.0, 1.0);

    Lo += CalculateDirectionalLight(u_DirectionalLight, V, color, normal, metallic, roughness);
    Lo *= (1 - shadow);

    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        Lo += CalculatePointLight(u_PointLights[i], position, V, color, normal, metallic, roughness);
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
        Lo += CalculateSpotLight(u_SpotLights[i], position, V, color, normal, metallic, roughness);

    vec3 ambient = vec3(0.03) * color * ao;

    vec3 lighting = ambient + Lo + emissive;
    f_Color = vec4(lighting, 1.0);
}