#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
};

layout (location = 0) uniform mat4 u_Model;

void main()
{
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_TexCoord = a_TexCoord;

    gl_Position = vec4(v_Position, 1.0);
}

#SHADER FRAGMENT
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
    
    if (u_IsGammaCorrection)
    {
        vec3 mapped = vec3(1.0) - exp(vec3(-image) * u_Exposure); //EXPOSURE TONE MAPPING
        mapped = pow(mapped, vec3(1.0 / u_Gamma));        //GAMMA CORRECTION

        vec4 final = vec4(mapped * u_Color.rgb, image.a * u_Color.a);
        f_Color = final;
    }
    else
    {
        image *= u_Color;
        f_Color = image;
    }
}