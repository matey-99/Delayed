#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 5) in mat4 a_Model;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoord;

struct Material
{
    vec4 RotateProjection;
    float NoiseScale;
    float NoiseSpeed;
};

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

layout (location = 0) uniform Material u_Material;
layout (location = 3) uniform float u_Time;

vec2 TilingAndOffset(vec2 texCoord, vec2 tiling, vec2 offset)
{
    return texCoord * tiling + offset;
}

vec3 RotateAboutAxis(vec3 vector, vec3 axis, float rotation)
{
    rotation = radians(rotation);

    float s = sin(rotation);
    float c = cos(rotation);
    float inversedCos = 1.0 - c;

    axis = normalize(axis);
    mat3 rot = 
    {
        { (1.0 - c) * axis.x * axis.x + c,            (1.0 - c) * axis.x * axis.y - axis.z * s,   (1.0 - c) * axis.z * axis.x + axis.y * s },
        { (1.0 - c) * axis.x * axis.y + axis.z * s,   (1.0 - c) * axis.y * axis.y + c,            (1.0 - c) * axis.y * axis.z - axis.x * s },
        { (1.0 - c) * axis.z * axis.x - axis.y * c,   (1.0 - c) * axis.y * axis.z + axis.x * s,   (1.0 - c) * axis.z * axis.z * c          }
    };


    return rot * vector;
}

vec2 GradientNoise_Dir(vec2 p)
{
    p = mod(p, 289);
    float x = mod(float(34 * p.x + 1) * p.x, 289) + p.y;
    x = mod((34 * x + 1) * x, 289);
    x = fract(x / 41) * 2 - 1;
    return normalize(vec2(x - floor(x + 0.5), abs(x) - 0.5));
}

float GradientNoise(vec2 UV, float Scale)
{
    vec2 p = UV * Scale;
    vec2 ip = floor(p);
    vec2 fp = fract(p);
    float d00 = dot(GradientNoise_Dir(ip), fp);
    float d01 = dot(GradientNoise_Dir(ip + vec2(0, 1)), fp - vec2(0, 1));
    float d10 = dot(GradientNoise_Dir(ip + vec2(1, 0)), fp - vec2(1, 0));
    float d11 = dot(GradientNoise_Dir(ip + vec2(1, 1)), fp - vec2(1, 1));
    fp = fp * fp * fp * (fp * (fp * 6 - 15) + 10);
    return mix(mix(d00, d01, fp.y), mix(d10, d11, fp.y), fp.x) + 0.5;
}

void main()
{
    v_Position = vec3(a_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(a_Model))) * a_Normal;
    v_TexCoord = a_TexCoord;

    Material m = u_Material;

    vec3 rotatedPos = RotateAboutAxis(v_Position, m.RotateProjection.xyz, m.RotateProjection.w);
    vec2 offset = vec2(m.NoiseSpeed * u_Time);

    vec2 uv = TilingAndOffset(rotatedPos.xy, vec2(1.0), offset);
    float noise = GradientNoise(uv, m.NoiseScale);

    vec3 displacement = v_Normal * vec3(noise);
    vec3 newPosition = v_Position + displacement;

    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;

struct Material
{
    vec4 RotateProjection;
    float NoiseScale;
    float NoiseSpeed;
};

layout (location = 0) uniform Material u_Material;
layout (location = 3) uniform float u_Time;

vec2 TilingAndOffset(vec2 texCoord, vec2 tiling, vec2 offset)
{
    return texCoord * tiling + offset;
}

vec3 RotateAboutAxis(vec3 vector, vec3 axis, float rotation)
{
    rotation = radians(rotation);

    float s = sin(rotation);
    float c = cos(rotation);
    float inversedCos = 1.0 - c;

    axis = normalize(axis);
    mat3 rot = 
    {
        { (1.0 - c) * axis.x * axis.x + c,            (1.0 - c) * axis.x * axis.y - axis.z * s,   (1.0 - c) * axis.z * axis.x + axis.y * s },
        { (1.0 - c) * axis.x * axis.y + axis.z * s,   (1.0 - c) * axis.y * axis.y + c,            (1.0 - c) * axis.y * axis.z - axis.x * s },
        { (1.0 - c) * axis.z * axis.x - axis.y * c,   (1.0 - c) * axis.y * axis.z + axis.x * s,   (1.0 - c) * axis.z * axis.z * c          }
    };

    return rot * vector;
}

vec2 GradientNoise_Dir(vec2 p)
{
    p = mod(p, 289);
    float x = mod(float(34 * p.x + 1) * p.x, 289) + p.y;
    x = mod((34 * x + 1) * x, 289);
    x = fract(x / 41) * 2 - 1;
    return normalize(vec2(x - floor(x + 0.5), abs(x) - 0.5));
}

float GradientNoise(vec2 UV, float Scale)
{
    vec2 p = UV * Scale;
    vec2 ip = floor(p);
    vec2 fp = fract(p);
    float d00 = dot(GradientNoise_Dir(ip), fp);
    float d01 = dot(GradientNoise_Dir(ip + vec2(0, 1)), fp - vec2(0, 1));
    float d10 = dot(GradientNoise_Dir(ip + vec2(1, 0)), fp - vec2(1, 0));
    float d11 = dot(GradientNoise_Dir(ip + vec2(1, 1)), fp - vec2(1, 1));
    fp = fp * fp * fp * (fp * (fp * 6 - 15) + 10);
    return mix(mix(d00, d01, fp.y), mix(d10, d11, fp.y), fp.x) + 0.5;
}

void main()
{
    Material m = u_Material;

    vec3 rotatedPos = RotateAboutAxis(v_Position, m.RotateProjection.xyz, m.RotateProjection.w);
    vec2 offset = vec2(m.NoiseSpeed * u_Time);

    vec2 uv = TilingAndOffset(rotatedPos.xy, vec2(1.0), offset);
    float noise = GradientNoise(uv, m.NoiseScale);

    f_Color = vec4(vec3(noise), 1.0);
}