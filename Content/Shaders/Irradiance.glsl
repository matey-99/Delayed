#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 v_LocalPosition;

layout (location = 0) uniform mat4 u_Projection;
layout (location = 1) uniform mat4 u_View;

void main()
{
    v_LocalPosition = a_Position;

    mat4 rotView = mat4(mat3(u_View));
    vec4 pos = u_Projection * rotView * vec4(v_LocalPosition, 1.0);
    
    gl_Position = pos.xyww;
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_LocalPosition;

layout (location = 2) uniform samplerCube u_EnvironmentMap;

const float PI = 3.14159265359;

void main()
{    
    vec3 normal = normalize(v_LocalPosition);
    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += texture(u_EnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    f_Color = vec4(irradiance, 1.0);
}