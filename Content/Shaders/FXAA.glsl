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

#define EDGE_THRESHOLD (1.0 / 8.0)
#define EDGE_THRESHOLD_MIN (1.0 / 16.0)
#define SEARCH_STEPS 16
#define SEARCH_SPEED 1
#define SUBPIXEL_QUALITY (3.0 / 4.0)

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_SourceTexture;
layout (location = 1) uniform vec2 u_InverseScreenSize; 

float Luma(vec3 rgb)
{
    return sqrt(dot(rgb, vec3(0.299, 0.587, 0.114)));
}

void main()
{   
    vec3 color = texture(u_SourceTexture, v_TexCoord).rgb;

    float lumaCenter = Luma(color);
    float lumaUp = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2(0.0, 1.0)).rgb);
    float lumaDown = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2(0.0, -1.0)).rgb);
    float lumaLeft = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2(-1.0, 0.0)).rgb);
    float lumaRight = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2(1.0, 0.0)).rgb);

    float lumaMin = min(lumaCenter, min(min(lumaUp, lumaDown), min(lumaLeft, lumaRight)));
    float lumaMax = max(lumaCenter, max(max(lumaUp, lumaDown), max(lumaLeft, lumaRight)));

    float lumaRange = lumaMax - lumaMin;
    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD))
    {
        f_Color = vec4(color, 1.0);
        return;
    }

    float lumaUpLeft = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2(-1.0,  1.0)).rgb);
    float lumaUpRight = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2( 1.0,  1.0)).rgb);
    float lumaDownLeft = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2(-1.0, -1.0)).rgb);
    float lumaDownRight = Luma(textureOffset(u_SourceTexture, v_TexCoord, ivec2( 1.0, -1.0)).rgb);
    
    float lumaUpDown = lumaUp + lumaDown;
    float lumaLeftRight = lumaLeft + lumaRight;

    float lumaUpCorners = lumaUpLeft + lumaUpRight;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaLeftCorners = lumaUpLeft + lumaDownLeft;
    float lumaRightCorners = lumaUpRight + lumaDownRight;

    float horizontalEdge = abs(-2.0 * lumaLeft + lumaLeftCorners) + abs(-2.0 * lumaCenter + lumaUpDown) * 2.0 + abs(-2.0 * lumaRight + lumaRightCorners);
    float verticalEdge = abs(-2.0 * lumaUp + lumaUpCorners) + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0 + abs(-2.0 * lumaDown + lumaDownCorners);

    bool isHorizontal = horizontalEdge >= verticalEdge;

    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;

    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));

    float stepLength = isHorizontal ? u_InverseScreenSize.y : u_InverseScreenSize.x;
    float lumaLocalAverage = 0.0;
    if (is1Steepest)
    {
        stepLength = -stepLength;
        lumaLocalAverage = 0.5 * (luma1 + lumaCenter);
    }
    else
    {
        lumaLocalAverage = 0.5 * (luma2 + lumaCenter);
    }

    vec2 currentUV = v_TexCoord.xy;
    if (isHorizontal)
        currentUV.y += stepLength * 0.5;
    else
        currentUV.x += stepLength * 0.5;

    vec2 offset = isHorizontal ? vec2(u_InverseScreenSize.x, 0.0) : vec2(0.0, u_InverseScreenSize.y);

    vec2 uv1 = currentUV - offset;
    vec2 uv2 = currentUV + offset;

    float lumaEnd1 = Luma(texture(u_SourceTexture, uv1).rgb);
    lumaEnd1 -= lumaLocalAverage;

    float lumaEnd2 = Luma(texture(u_SourceTexture, uv2).rgb);
    lumaEnd2 -= lumaLocalAverage;

    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    if (!reached1)
        uv1 -= offset;
    if (!reached2)
        uv2 += offset;

    if (!reachedBoth)
    {
        for (int i = 2; i < SEARCH_STEPS; i++)
        {
            if (!reached1)
            {
                lumaEnd1 = Luma(texture(u_SourceTexture, uv1).rgb);
                lumaEnd1 -= lumaLocalAverage;
            }
            if (!reached2)
            {
                lumaEnd2 = Luma(texture(u_SourceTexture, uv2).rgb);
                lumaEnd2 -= lumaLocalAverage;
            }

            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

            if (!reached1)
                uv1 -= offset * SEARCH_SPEED;
            if (!reached2)
                uv2 += offset * SEARCH_SPEED;

            if (reachedBoth)
                break;
        }
    }

    float distance1 = isHorizontal ? (v_TexCoord.x - uv1.x) : (v_TexCoord.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - v_TexCoord.x) : (uv2.y - v_TexCoord.y);

    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    float edgeThickness = distance1 + distance2;

    float pixelOffset = -distanceFinal / edgeThickness + 0.5;

    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;
    
    float finalOffset = correctVariation ? pixelOffset : 0.0;

    // subpixel shifting
    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaUpDown + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

    float subpixelOffset1 = clamp(abs(lumaAverage - lumaCenter) / lumaRange, 0.0, 1.0);
    float subpixelOffset2 = (-2.0 * subpixelOffset1 + 3.0) * subpixelOffset1 * subpixelOffset1;

    float subpixelOffsetFinal = subpixelOffset2 * subpixelOffset2 * SUBPIXEL_QUALITY;

    finalOffset = max(finalOffset, subpixelOffsetFinal);

    vec2 finalUV = v_TexCoord;
    if (isHorizontal)
        finalUV.y += finalOffset * stepLength;
    else
        finalUV.x += finalOffset * stepLength;

    vec3 finalColor = texture(u_SourceTexture, finalUV).rgb;
    f_Color = vec4(finalColor, 1.0);
}