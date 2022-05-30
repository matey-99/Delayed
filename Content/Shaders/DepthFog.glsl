#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;

    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_GBufferPosition;
layout (location = 1) uniform sampler2D u_Screen;

layout (location = 3) uniform float u_Density;
layout (location = 4) uniform float u_Height;
layout (location = 5) uniform vec3 u_FogColor;

layout (std140, binding = 2) uniform u_FragmentCamera
{
    vec3 u_ViewPosition;
    float u_CameraFarClipPlane;
};

void main()
{
    vec4 color = texture(u_Screen, v_TexCoord);
    vec4 fragmentPosition = texture(u_GBufferPosition, v_TexCoord);
    vec3 viewDirection = normalize(vec3(fragmentPosition) - u_ViewPosition);

    // Calculate fog
    float dist = length(vec3(fragmentPosition) - u_ViewPosition);

    float fogFactor = (u_Density / u_Height) * exp( -u_ViewPosition.y * u_Height) * (1.0 - exp( -dist * viewDirection.y * u_Height )) / viewDirection.y;
    fogFactor *= 1.0 - exp( -dist * u_Density );

    fogFactor = clamp(fogFactor, 0.0, 1.0);

    f_Color = mix(color, vec4(u_FogColor, 1.0), fogFactor);
}