#version 450 core

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec4 v_DirectionalLightSpacePosition;
layout (location = 4) in vec4[MAX_SPOT_LIGHTS] v_SpotLightSpacePositions;

struct Material
{
    bool isNormalMap;

    sampler2D grassTexture;
    sampler2D grassNormalMap;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;

    bool shadowsEnabled;
};

struct PointLight
{
    vec3 position;
    vec3 color;

    bool shadowsEnabled;
    float farPlane;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float innerCutOff;
    float outerCutOff;

    bool shadowsEnabled;
};

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
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
layout (location = 5) uniform bool u_IsSkyLight;
layout (location = 6) uniform float u_SkyLightIntensity;
layout (location = 7) uniform samplerCube u_IrradianceMap;
layout (location = 8) uniform samplerCube u_PrefilterMap;
layout (location = 9) uniform sampler2D u_BRDFLUT;

const float PI = 3.14159265359;

vec3 GetNormalFromNormalMap()
{
    vec3 tangentNormal = texture(u_Material.grassNormalMap, v_TexCoord).xyz * 2.0 - 1.0;

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

void main()
{
    vec4 textureColor = texture(u_Material.grassTexture, v_TexCoord);
    if (textureColor.a < 0.5)
        discard;

    vec3 albedo = pow(textureColor.rgb, vec3(2.2));

/*     vec3 N;
    if (u_Material.isNormalMap)
        N = GetNormalFromNormalMap();
    else
        N = normalize(v_Normal);

    float metallic = 0.0;
    float roughness = 0.0;
    float ao = 1.0;

    vec3 V = normalize(u_ViewPosition - v_Position);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    Lo += CalculateDirectionalLight(u_DirectionalLight, V, albedo, N, metallic, roughness);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        Lo += CalculatePointLight(u_PointLights[i], V, albedo, N, metallic, roughness);
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
        Lo += CalculateSpotLight(u_SpotLights[i], V, albedo, N, metallic, roughness);

    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 diffuse, specular;

    if (u_IsSkyLight)
    {
        vec3 irradiance = texture(u_IrradianceMap, N).rgb;
        diffuse = irradiance * albedo * u_SkyLightIntensity;

        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureLod(u_PrefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf = texture(u_BRDFLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
        specular = prefilteredColor * (F * brdf.x + brdf.y) * u_SkyLightIntensity;
    }
    else
    {
        diffuse = vec3(0.0);
        specular = vec3(0.0);
    }

    vec3 ambient = (kD * diffuse + specular) * ao;
    vec3 color = ambient + Lo;
 */
    f_Color = vec4(albedo, 1.0);
}