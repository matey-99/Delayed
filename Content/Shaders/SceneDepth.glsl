#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 5) in mat4 a_Model;
layout (location = 9) in ivec4 a_BoneIds;
layout (location = 10) in vec4 a_Weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(a_BoneIds[i] == -1) 
            continue;
        if(a_BoneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(a_Position, 1.0);
            break;
        }
        vec4 localPosition = finalBonesMatrices[a_BoneIds[i]] * vec4(a_Position, 1.0);
        totalPosition += localPosition * a_Weights[i];
    }

    vec3 position = length(totalPosition) > 0.0 ? vec3(totalPosition) : a_Position;

    gl_Position = a_Model * vec4(position, 1.0);
}

#SHADER GEOMETRY
#version 450 core

#define MAX_SPOT_LIGHTS 16

layout (triangles, invocations = 5) in;
layout (triangle_strip, max_vertices = 3) out;

layout (std140, binding = 1) uniform u_VertexLights
{
    mat4 u_DirectionalLightSpaceMatrices[16];
    mat4[MAX_SPOT_LIGHTS] u_SpotLightSpaceMatrices;
};

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gl_Position = u_DirectionalLightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}

#SHADER FRAGMENT
#version 450 core

void main()
{   

}