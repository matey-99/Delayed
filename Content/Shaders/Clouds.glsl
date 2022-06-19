#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 5) in mat4 a_Model;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoord;
layout (location = 3) out vec4 v_ViewPosition;

struct Material
{
    vec4 RotateProjection;
    float NoiseScale;
    float NoiseSpeed;
    float NoiseHeight;
    vec4 NoiseRemap;
    vec4 ColorValley;
    vec4 ColorPeak;
    float NoiseEdge1;
    float NoiseEdge2;
    float NoisePower;
    float BaseScale;
    float BaseSpeed;
    float BaseStrength;
    float EmissionStrength;
    float CurvatureRadius;
    float FresnelPower;
    float FresnelOpacity;
    float CloudsDensity;
};

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

layout (location = 0) uniform Material u_Material;
layout (location = 50) uniform float u_Time;

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

float Remap(float value, vec2 inMinMax, vec2 outMinMax)
{
    return outMinMax.x + (value - inMinMax.x) * (outMinMax.y - outMinMax.x) / (inMinMax.y - inMinMax.x);
}

void main()
{
    v_Position = vec3(a_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(a_Model))) * a_Normal;
    v_TexCoord = a_TexCoord;
    v_ViewPosition = u_View * vec4(v_Position, 1.0);

    Material m = u_Material;

    vec3 rotatedPos = RotateAboutAxis(v_Position, m.RotateProjection.xyz, m.RotateProjection.w);
    vec2 offset = vec2(m.NoiseSpeed * u_Time);

    vec2 uv1 = TilingAndOffset(rotatedPos.xy, vec2(1.0), offset);
    float gradientNoise1 = GradientNoise(uv1, m.NoiseScale);

    vec2 uv2 = TilingAndOffset(rotatedPos.xy, vec2(1.0), vec2(0.0));
    float gradientNoise2 = GradientNoise(uv2, m.NoiseScale);

    float noise = (gradientNoise1 + gradientNoise2) * 0.5;
    noise = clamp(noise, 0.0, 1.0);
    noise = pow(noise, m.NoisePower);
    noise = Remap(noise, m.NoiseRemap.xy, m.NoiseRemap.zw);
    noise = abs(noise);
    noise = smoothstep(m.NoiseEdge1, m.NoiseEdge2, noise);

    vec2 baseOffset = vec2(m.BaseSpeed * u_Time);
    vec2 baseUV = TilingAndOffset(rotatedPos.xy, vec2(1.0), baseOffset);
    float base = GradientNoise(baseUV, m.BaseScale);
    base *= m.BaseStrength;

    noise += base;
    noise /= (1.0 + m.BaseStrength);

    vec3 displacement = v_Normal * vec3(noise) * m.NoiseHeight;
    vec3 newPosition = v_Position + displacement;

    vec3 objectPosition = vec3(a_Model[0][3], a_Model[1][3], a_Model[2][3]);
    float dist = distance(objectPosition, v_Position);
    dist /= m.CurvatureRadius;
    dist = pow(dist, 3.0);
    vec3 displacement2 = v_Normal * dist;
    newPosition += displacement2;

    gl_Position = u_ViewProjection * vec4(newPosition, 1.0);
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
    vec4 RotateProjection;
    float NoiseScale;
    float NoiseSpeed;
    float NoiseHeight;
    vec4 NoiseRemap;
    vec4 ColorValley;
    vec4 ColorPeak;
    float NoiseEdge1;
    float NoiseEdge2;
    float NoisePower;
    float BaseScale;
    float BaseSpeed;
    float BaseStrength;
    float EmissionStrength;
    float CurvatureRadius;
    float FresnelPower;
    float FresnelOpacity;
    float CloudsDensity;
};

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
    float u_CameraFarClipPlane;
};

layout (location = 0) uniform Material u_Material;
layout (location = 50) uniform float u_Time;

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

float Remap(float value, vec2 inMinMax, vec2 outMinMax)
{
    return outMinMax.x + (value - inMinMax.x) * (outMinMax.y - outMinMax.x) / (inMinMax.y - inMinMax.x);
}

float Fresnel(vec3 normal, vec3 viewDirection, float power)
{
    return pow((1.0 - clamp(dot(normalize(normal), normalize(viewDirection)), 0.0, 1.0)), power);
}

void main()
{
    Material m = u_Material;

    vec3 rotatedPos = RotateAboutAxis(v_Position, m.RotateProjection.xyz, m.RotateProjection.w);
    vec2 offset = vec2(m.NoiseSpeed * u_Time);

    vec2 uv1 = TilingAndOffset(rotatedPos.xy, vec2(1.0), offset);
    float gradientNoise1 = GradientNoise(uv1, m.NoiseScale);

    vec2 uv2 = TilingAndOffset(rotatedPos.xy, vec2(1.0), vec2(0.0));
    float gradientNoise2 = GradientNoise(uv2, m.NoiseScale);

    float noise = (gradientNoise1 + gradientNoise2) * 0.5;
    noise = clamp(noise, 0.0, 1.0);
    noise = pow(noise, m.NoisePower);
    noise = Remap(noise, m.NoiseRemap.xy, m.NoiseRemap.zw);
    noise = abs(noise);
    noise = smoothstep(m.NoiseEdge1, m.NoiseEdge2, noise);

    vec2 baseOffset = vec2(m.BaseSpeed * u_Time);
    vec2 baseUV = TilingAndOffset(rotatedPos.xy, vec2(1.0), baseOffset);
    float base = GradientNoise(baseUV, m.BaseScale);
    base *= m.BaseStrength;

    noise += base;
    noise /= (1.0 + m.BaseStrength);

    vec3 color = mix(m.ColorValley, m.ColorPeak, noise).rgb;

    vec3 normal = v_Normal;
    vec3 viewDirection = u_ViewPosition - v_Position;
    float fresnel = noise * Fresnel(normal, viewDirection, m.FresnelPower) * m.FresnelOpacity;

    color += vec3(fresnel);
    color *= m.EmissionStrength;

    float depth = gl_FragCoord.z;
    depth -= (v_ViewPosition.z - 1.0);
    float density = depth / m.CloudsDensity;
    density = clamp(density, 0.0, 1.0);
    density = smoothstep(0.0, 1.0, density);

    f_Color = vec4(color, density);
}