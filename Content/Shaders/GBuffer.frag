#version 450 core

layout (location = 0) out vec4 f_Position;
layout (location = 1) out vec4 f_Normal;
layout (location = 2) out vec4 f_ColorAO;
layout (location = 3) out vec4 f_MetallicRoughness;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;

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

    f_MetallicRoughness = vec4(metallic, roughness, 0.0, 1.0);
}