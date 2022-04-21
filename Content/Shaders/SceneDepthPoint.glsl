#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 5) in mat4 a_Model;

void main()
{
    gl_Position = a_Model * vec4(a_Position, 1.0);
}


#SHADER GEOMETRY
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

#SHADER FRAGMENT
#version 450 core

in vec4 g_Position;

layout (location = 7) uniform vec3 u_LightPos;
layout (location = 8) uniform float u_FarPlane;

void main()
{   
    float lightDistance = length(g_Position.xyz - u_LightPos);
    lightDistance = lightDistance / u_FarPlane;
    gl_FragDepth = lightDistance;
}