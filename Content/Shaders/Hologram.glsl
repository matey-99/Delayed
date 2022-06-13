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

layout (location = 0) uniform Material u_MaterialVS;

void main()
{
    v_Position = vec3(a_Model * vec4(a_Position, 1.0));
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
    vec3 fresnelColor;
    vec3 mainColor;

    sampler2D hologramLines;

    float fresnelPower;
    float scrollSpeed;
    float tiling;
};

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
    float u_CameraFarClipPlane;
};

layout (location = 2) uniform Material u_Material;
layout (location = 10) uniform float u_Time;

const float PI = 3.14159265359;

float Saturate(float val) {
    return clamp(val, 0.0, 1.0);
}

float FresnelEffect(vec3 Normal, vec3 ViewDir, float Power)
{
    return pow((1.0 - Saturate(dot(normalize(Normal), normalize(ViewDir)))), Power);
}

vec2 TilingAndOffset(vec2 UV, vec2 Tiling, vec2 Offset)
{
    return UV * Tiling + Offset;
}

void main()
{
    vec3 viewDir = normalize(u_ViewPosition - v_Position);
    float fresnelEffect = FresnelEffect(v_Normal, viewDir, u_Material.fresnelPower);
    vec4 fresnelColor = vec4(u_Material.fresnelColor, 1.0) * fresnelEffect;

    float scroll = u_Material.scrollSpeed * u_Time;
    scroll += v_Position.y;
    vec2 uv = vec2(0, scroll);
    vec4 lines = texture(u_Material.hologramLines, TilingAndOffset(uv, vec2(u_Material.tiling), vec2(0, 0)));

    vec4 effect = fresnelColor * lines;

    vec3 color = u_Material.mainColor + effect.rgb;

    f_Color = vec4(color, effect.a);
}