#version 450 core

layout (triangles) in;

layout (triangle_strip, max_vertices = 18) out;

layout (location = 1) uniform mat4 u_ShadowMatrices[6];

out vec4 g_Position;

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for (int i = 0; i < 3; ++i)
        {
            g_Position = gl_in[i].gl_Position;
            gl_Position = u_ShadowMatrices[face] * g_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}