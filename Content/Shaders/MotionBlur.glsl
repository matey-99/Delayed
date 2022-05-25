#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_Position;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Position = a_Position;

    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Position;

layout (location = 0) uniform sampler2D u_Screen;  // colorTexture
layout (location = 1) uniform sampler2D u_GBufferViewPosition;  // positionTexture

layout (location = 2) uniform int u_Size;
layout (location = 3) uniform float u_Separation;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;  // lensProjection or worldViewMat here
    mat4 u_View;            // or maybe worldViewMat there
    mat4 u_Projection;      // or lensProjection there
    mat4 u_PreviousView;    // or maybe worldViewMat there
    mat4 u_PreviousViewProjection;    // or maybe worldViewMat there
};

void main()
{
    mat4 lensProjection = u_Projection;

    vec2 texSize  = textureSize(u_Screen, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    f_Color = texture(u_Screen, v_TexCoord);
    vec4 position1 = vec4(v_Position, 1.0);
    //vec4 position0 = u_View * previousViewWorldMat * position1;  // previousViewWorldMat
    //vec4 position0 = u_View * u_PreviousView * position1;  // previousViewWorldMat
    vec4 position0 = inverse(u_View) * position1;  // previousViewWorldMat

    position0      = lensProjection * position0;
    position0.xyz /= position0.w;
    position0.xy   = position0.xy * 0.5 + 0.5;

    position1      = lensProjection * position1;
    position1.xyz /= position1.w;
    position1.xy   = position1.xy * 0.5 + 0.5;

    vec2 direction    = position1.xy - position0.xy;
    if (length(direction) <= 0.0)
        { return; }

    direction.xy *= u_Separation;

    vec2  forward  = v_TexCoord;
    vec2  backward = v_TexCoord;

    float count = 1.0;

    for (int i = 0; i < u_Size; ++i)
    {
        forward  += direction;
        backward -= direction;

        f_Color += texture(u_Screen,  forward);
        f_Color += texture(u_Screen, backward);

        count += 2.0;
    }

    f_Color /= count;


    //f_Color = vec4(1.0, 0.5, 0.9, 1.0);
}
