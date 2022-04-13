#version 450 core

layout (location = 0) out vec4 f_Color;

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) uniform sampler2D u_SourceTexture;

void main()
{  
    vec3 color = texture(u_SourceTexture, v_TexCoord).rgb;

    vec2 uv = v_TexCoord.xy ;
   
    uv *=  vec2(1.0) - uv.yx;   
    
    float vig = uv.x*uv.y * 15.0; 
    
    vig = pow(vig, 0.25); 

    color*=vec3(vig);

    f_Color = vec4(color, 1.0); 
}