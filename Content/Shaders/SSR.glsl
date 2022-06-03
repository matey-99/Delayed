#SHADER VERTEX
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;

    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#SHADER FRAGMENT
#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 11) uniform sampler2D u_Screen;
layout (location = 0) uniform sampler2D u_GBufferViewPosition;
layout (location = 1) uniform sampler2D u_GBufferNormal;
layout (location = 3) uniform vec2 u_ScreenSize;
layout (location = 4) uniform sampler2D u_GBufferMetallicRoughness;

layout (location = 5) uniform float u_RayStep;
layout (location = 6) uniform float u_MinRayStep;
layout (location = 7) uniform int u_MaxSteps;
layout (location = 8) uniform int u_NumBinarySearchSteps;
layout (location = 9) uniform float u_ReflectionSpecularFalloffExponent;

layout (std140, binding = 0) uniform u_VertexCamera
{
    mat4 u_ViewProjection;
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_InvView;
};

float Metallic;
#define Scale vec3(.8, .8, .8)
#define K 19.19

vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 hash(vec3 a);

void main()
{
    /*
    vec3 col = vec3(texture(u_GBufferMetallicRoughness, v_TexCoord).r);
    f_Color = vec4(col, 1.0);
    return;
    */

    vec2 MetallicRoughness = texture2D(u_GBufferMetallicRoughness, v_TexCoord).rg;
    Metallic = MetallicRoughness.r;

    if(Metallic < 0.01)
        discard;

    //vec3 viewNormal = vec3(texture2D(u_GBufferNormal, v_TexCoord));
    vec3 viewNormal = vec3(texture2D(u_GBufferNormal, v_TexCoord) * inverse(u_View));  // (originally invView) - transforms world space normal vector to view space
    //vec3 viewPos = vec3(texture2D(u_GBufferViewPosition, v_TexCoord));
    vec3 viewPos = texture(u_GBufferViewPosition, v_TexCoord).xyz;
    vec3 albedo = texture(u_Screen, v_TexCoord).rgb;

    float spec = texture(u_GBufferMetallicRoughness, v_TexCoord).g;  // float spec = texture(ColorBuffer, TexCoords).w;

    vec3 F0 = vec3(0.04);
    F0      = mix(F0, albedo, Metallic);
    vec3 Fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0), F0);

    // Reflection vector
    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));
    
    vec3 hitPos = viewPos;  // position where ray hits the surface
    float dDepth;
 
    // f_Color = vec4(reflected, 1.0);  // visualize reflections

    vec3 wp = vec3(vec4(viewPos, 1.0) * inverse(u_View));  // originally invView
    vec3 jitt = mix(vec3(0.0), vec3(hash(wp)), spec);
    vec4 coords = RayMarch((vec3(jitt) + reflected * max(u_MinRayStep, -viewPos.z)), hitPos, dDepth);
 
    vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - coords.xy));
 
    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

    float ReflectionMultiplier = pow(Metallic, u_ReflectionSpecularFalloffExponent) * 
                screenEdgefactor * 
                -reflected.z;

    // Get color
    vec3 SSR = textureLod(u_Screen, coords.xy, 0).rgb * clamp(ReflectionMultiplier, 0.0, 0.9) * Fresnel;
    
    f_Color = vec4(SSR, Metallic);

    // Gaussian blur
    
    /*
    vec2 pos = gl_FragCoord.xy/u_ScreenSize;
    pos.x = (pos.x * 2) - 1.0;
    pos.y = (pos.y * 2) - 1.0;
    float xs = u_ScreenSize.x;
    float ys = u_ScreenSize.y;
    float r = 15;
    float x,y,xx,yy,rr=r*r,dx,dy,w,w0;
    w0=0.3780/pow(r,1.975);
    vec2 p;
    vec4 col=vec4(0.0,0.0,0.0,0.0);
    for (dx=1.0/xs,x=-r,p.x=0.5+(pos.x*0.5)+(x*dx);x<=r;x++,p.x+=dx){ xx=x*x;
     for (dy=1.0/ys,y=-r,p.y=0.5+(pos.y*0.5)+(y*dy);y<=r;y++,p.y+=dy){ yy=y*y;
      if (xx+yy<=rr)
        {
        w=w0*exp((-xx-yy)/(2.0*rr));
        col+=texture2D(u_Screen, p)*w;
        }}}
    

    //f_Color = vec4(position.x, position.y, 0.0, 1.0);
    f_Color = (texture2D(u_Screen, v_TexCoord).rg, 1.0, 1.0);
    */
}


vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
{
    float depth;

    vec4 projectedCoord;
 
    for(int i = 0; i < u_NumBinarySearchSteps; i++)
    {
        projectedCoord = u_Projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(u_GBufferViewPosition, projectedCoord.xy, 2).z;
 
        dDepth = hitCoord.z - depth;

        dir *= 0.5;
        if(dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;    
    }

        projectedCoord = u_Projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
    return vec3(projectedCoord.xy, depth);
}

vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    dir *= u_RayStep;
 
    float depth;
    int steps;
    vec4 projectedCoord;

    for(int i = 0; i < u_MaxSteps; i++)
    {
        hitCoord += dir;
 
        projectedCoord = u_Projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = texture(u_GBufferViewPosition, projectedCoord.xy, 2).z;
        if(depth > 1000.0)
            continue;
 
        dDepth = hitCoord.z - depth;

        if((dir.z - dDepth) < 1.2)
        {
            if(dDepth <= 0.0)
            {   
                vec4 Result;
                Result = vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);

                return Result;
            }
        }
        
        steps++;
    }
 
    return vec4(projectedCoord.xy, depth, 0.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 hash(vec3 a)
{
    a = fract(a * Scale);
    a += dot(a, a.yxz + K);
    return fract((a.xxy + a.yxx)*a.zyx);
}