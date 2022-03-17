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