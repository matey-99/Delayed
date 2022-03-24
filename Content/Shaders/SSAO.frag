#version 450 core

layout (location = 0) out float f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_GBufferPosition;
layout (location = 1) uniform sampler2D u_GBufferNormal;
layout (location = 2) uniform sampler2D u_NoiseTexture;
layout (location = 3) uniform vec2 u_ScreenSize;
layout (location = 4) uniform int u_KernelSize;
layout (location = 5) uniform float u_Radius;
layout (location = 6) uniform float u_Bias;
layout (location = 7) uniform float u_Intensity;
layout (location = 8) uniform vec3 u_Kernel[64];

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

void main()
{    
    const vec2 noiseScale = vec2(u_ScreenSize.x / 4.0, u_ScreenSize.y / 4.0);

    vec3 position = texture(u_GBufferPosition, v_TexCoord).rgb;
    vec3 normal = normalize(texture(u_GBufferNormal, v_TexCoord).rgb);
    vec3 random = normalize(texture(u_NoiseTexture, v_TexCoord * noiseScale).rgb);

    vec3 tangent = normalize(random - normal * dot(random, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < u_KernelSize; i++)
    {
        vec3 samplePos = TBN * u_Kernel[i];
        samplePos = position + samplePos * u_Radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = u_Projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(u_GBufferPosition, offset.xy).z;
        float rangeCheck = smoothstep(0.0, 1.0, u_Radius / abs(position.z - sampleDepth));

        occlusion += (sampleDepth >= samplePos.z + u_Bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / u_KernelSize);

    f_Color = pow(occlusion, u_Intensity);
}