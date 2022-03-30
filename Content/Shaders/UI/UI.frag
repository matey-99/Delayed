#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;

layout (location = 1) uniform sampler2D u_Image;
layout (location = 2) uniform vec4 u_Color;
layout (location = 3) uniform bool u_IsGammaCorrection;
layout (location = 4) uniform float u_Gamma;
layout (location = 5) uniform float u_Exposure;

void main()
{    
    vec4 image = texture(u_Image, v_TexCoord);
    image *= u_Color;

    if (u_IsGammaCorrection)
    {
        vec3 mapped = vec3(1.0) - exp(vec3(-image) * u_Exposure); //EXPOSURE TONE MAPPING
        mapped = pow(mapped, vec3(1.0 / u_Gamma));        //GAMMA CORRECTION

        f_Color = vec4(mapped, image.a);
        return;
    }
    else
        f_Color = image;
}