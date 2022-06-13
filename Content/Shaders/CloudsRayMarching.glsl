#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;
layout (location = 5) in mat4 a_Model;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoord;
layout (location = 3) out vec4 v_ViewPosition;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

void main()
{
    v_Position = vec3(a_Model * vec4(a_Position, 1.0));
    v_ViewPosition = u_View * vec4(v_Position, 1.0);
    v_Normal = mat3(transpose(inverse(a_Model))) * a_Normal;
    v_TexCoord = a_TexCoord;

    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec4 v_ViewPosition;

struct Material
{
    vec3 Color;
    float NumSteps;
    float CloudScale;
    float DensityMultiplier;
    float DensityThreshold;
    vec3 CloudOffset;

    vec3 BoundsMin;
    vec3 BoundsMax;
};

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
    float u_CameraFarClipPlane;
};

layout (location = 2) uniform Material u_Material;
layout (location = 10) uniform sampler3D u_NoiseTexture;

vec2 RayBoxDst(vec3 boundsMin, vec3 boundsMax, vec3 rayOrigin, vec3 rayDirection)
{
    vec3 t0 = (boundsMin - rayOrigin) / rayDirection;
    vec3 t1 = (boundsMax - rayOrigin) / rayDirection;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    float dstA = max(max(tmin.x, tmin.y), tmin.z);
    float dstB = min(min(tmax.x, tmax.y), tmax.z); // if something doesn't work, try change this line

    float dstToBox = max(0, dstA);
    float dstInsideBox = max(0, dstB - dstToBox);
    
    return vec2(dstToBox, dstInsideBox);
}

float LinearEyeDepth(float rawDepth)
{
    float near = 0.1;
    float far = 500.0;

    float x, y, z, w;
    x = -1.0 + near / far;
    y = 1.0;
    z = x / near;
    w = 1.0 / near;

    return 1.0 / (z * rawDepth + w);
}

float SampleDensity(vec3 position)
{
    vec3 uvw = position * u_Material.CloudScale * 0.001 + u_Material.CloudOffset * 0.01;
    vec4 shape = texture(u_NoiseTexture, uvw);
    float density = max(0, shape.r - u_Material.DensityThreshold) * u_Material.DensityMultiplier;
    return density;
}

void main()
{
    vec3 color = u_Material.Color;

    vec3 rayOrigin = u_ViewPosition;
    vec3 rayDir = normalize(v_Position - u_ViewPosition);

    vec2 rayBoxInfo = RayBoxDst(u_Material.BoundsMin, u_Material.BoundsMax, rayOrigin, rayDir);
    float dstToBox = rayBoxInfo.x;
    float dstInsideBox = rayBoxInfo.y;

    float dstTravelled = 0.0;
    float stepSize = dstInsideBox / u_Material.NumSteps;
    float dstLimit = min(dstToBox, dstInsideBox);

    float totalDensity = 0;
    while (dstTravelled < dstLimit)
    {
        vec3 rayPos = rayOrigin + rayDir * (dstToBox + dstTravelled);
        totalDensity += SampleDensity(rayPos) * stepSize;
        dstTravelled += stepSize;
    }

    float transmittance = exp(-totalDensity);

    f_Color = vec4(color, 1.0 - transmittance);
}