#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_LocalPosition;

layout (location = 2) uniform sampler2D u_EquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{   
    vec2 uv = SampleSphericalMap(normalize(v_LocalPosition));
    vec3 color = texture(u_EquirectangularMap, uv).rgb;

    f_Color = vec4(color, 1.0);
}