#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_LocalPosition;

layout (location = 2) uniform samplerCube u_EnvironmentMap;

void main()
{    
    vec3 envColor = textureLod(u_EnvironmentMap, v_LocalPosition, 1.2).rgb;

    f_Color = vec4(envColor, 1.0);
}