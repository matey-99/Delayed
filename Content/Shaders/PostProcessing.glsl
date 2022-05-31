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

#define sat(x) clamp(x,0.,1.)

layout (location = 0) uniform sampler2D u_Screen;
layout (location = 1) uniform sampler2D u_BloomTexture;
layout (location = 2) uniform bool u_IsBloom;
layout (location = 3) uniform float u_BloomIntensity;
layout (location = 4) uniform float u_Gamma;
layout (location = 5) uniform float u_Exposure;
layout (location = 6) uniform float u_Saturation;
layout (location = 7) uniform float u_Temperature;
layout (location = 8) uniform float u_Hue;

layout (location = 9) uniform float u_Offset;
layout (location = 10) uniform float u_Lift;
layout (location = 11) uniform float u_Gain;

layout (location = 12) uniform float u_Contrast;
layout (location = 13) uniform float u_ContrastPivot;

layout (location = 14) uniform bool u_IsAberration;
layout (location = 15) uniform vec3 u_AberrationShift;


// https://gist.github.com/sugi-cho/6a01cae436acddd72bdf
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0,-1/3.,2/3.,-1),
    p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g)),
    q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    float d = q.x - min(q.w, q.y), e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    return c.z * mix(vec3(1), sat(abs(fract(vec3(1,2/3., 1/3.) + c.x) * 6 - 3) - 1), c.y);
}

vec3 rgb2hsv(float r, float g, float b)
{
    return rgb2hsv(vec3(r, g, b));
}

vec3 hsv2rgb(float h, float s, float v)
{
    return hsv2rgb(vec3(h, s, v));
}

// from http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
vec3 colorFromKelvin(float temperature) // photographic temperature values are between 15 to 150
{
    float r, g, b;

    if (temperature <= 66.0)
    {
        r = 1.0;
        g = sat((99.4708025861 * log(temperature) - 161.1195681661) / 255.0);
        if (temperature < 19.0)
            b = 0.0;
        else
            b = sat((138.5177312231 * log(temperature - 10.0) - 305.0447927307) / 255.0);
    }
    else
    {
        r = sat((329.698727446 / 255.0) * pow(temperature - 60.0, -0.1332047592));
        g = sat((288.1221695283  / 255.0) * pow(temperature - 60.0, -0.0755148492));
        b = 1.0;
    }

    return vec3(r, g, b);
}

// Good & fast sRgb approximation from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html
vec3 LinearToSRGB(vec3 rgb)
{
    rgb=max(rgb,vec3(0,0,0));
    return max(1.055*pow(rgb,vec3(0.416666667))-0.055,0.0);
}

float Luma(vec3 color) { return dot(color, vec3(0.2126, 0.7152, 0.0722)); }

void main()
{
    vec3 col;
    if (u_IsAberration) {
        col.r = texture(u_Screen, v_TexCoord + u_AberrationShift.x).r;
        col.g = texture(u_Screen, v_TexCoord + u_AberrationShift.y).g;
        col.b = texture(u_Screen, v_TexCoord + u_AberrationShift.z).b;
    } else {
        col = texture(u_Screen, v_TexCoord).rgb;
    }

    // SATURATION
    col = sat(col);
    float luma = Luma(col);

    // CONTRAST
    col = mix(vec3(u_ContrastPivot), col, sat(u_Contrast));
	
	vec3 p = vec3(1.0 / sat(2.0 - u_Contrast));
	vec3 dark = pow(col / u_ContrastPivot, p) * u_ContrastPivot;
	vec3 ip = vec3(1.0 - u_ContrastPivot);
	vec3 light = 1.0 - pow(1.0 / ip - col / ip, p) * ip;
	col = mix(dark, light, greaterThan(col, vec3(u_ContrastPivot)));

    // BLOOM
    if (u_IsBloom)
        col += u_BloomIntensity * texture(u_BloomTexture, v_TexCoord).rgb;

    // SATURATION C.D.
	col = mix(vec3(luma), col, u_Saturation);
	//col.r = mix(luma, col.r, 0.0f);

    // TEMPERATURE
    col *= vec3(1.0) / colorFromKelvin(u_Temperature);

    // HUE
    col = hsv2rgb(rgb2hsv(col) + vec3(fract((u_Hue - 180.0) / 360.0), 0.0, 0.0));

    //vec3 mapped = col / (col + vec3(1.0));          //REINHARD TONE MAPPING
    
    col = vec3(1.0) - exp(-col * u_Exposure); //EXPOSURE TONE MAPPING
    //mapped = pow(mapped, vec3(1.0 / u_Gamma));        //GAMMA CORRECTION

    // NEW THREE-WAY COLOR CORRECTOR (GAMMA INCLUDED)
    col = pow(max(vec3(0.0), col * (1.0 + u_Gain - u_Lift) + u_Lift + u_Offset), max(vec3(0.0), 1.0 - u_Gamma));

    col = LinearToSRGB(col); // ACESFilm(v * uExposure)

    //f_Color = vec4(mapped, 1.0);
    f_Color = vec4(col, 1.0);
}