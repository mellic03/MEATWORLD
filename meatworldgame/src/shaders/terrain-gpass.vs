#version 460 core

#extension GL_GOOGLE_include_directive: require
#include "./include/storage.glsl"


layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec3 fsin_tangent;
out vec2 fsin_texcoords;
flat out uint draw_id;

out vec3 TBN_viewpos;
out vec3 TBN_fragpos;
out mat3 TBN;
out mat3 TBNT;


vec3 mod289 ( vec3 x ) { return x - floor ( x * (1.0 / 289.0) ) * 289.0; }
vec2 mod289 ( vec2 x ) { return x - floor ( x * (1.0 / 289.0) ) * 289.0; }
vec3 permute ( vec3 x ) { return mod289 ((( x *34.0) +1.0) * x ) ; }

float snoise( vec2 v )
{
    const vec4 C = vec4 (0.211324865405187 , // (3.0 - sqrt (3.0) ) /6.0
    0.366025403784439 , // 0.5*( sqrt (3.0) -1.0)
    -0.577350269189626 , // -1.0 + 2.0 * C . x
    0.024390243902439) ; // 1.0 / 41.0
    // First corner
    vec2 i = floor ( v + dot (v , C . yy ) ) ;
    vec2 x0 = v - i + dot (i , C . xx ) ;
    // Other corners
    vec2 i1 = ( x0 . x > x0 . y ) ? vec2 (1.0 , 0.0) : vec2 (0.0 , 1.0) ;
    vec4 x12 = x0 . xyxy + C . xxzz ;
    x12 . xy -= i1 ;
    // Permutations
    i = mod289 ( i ) ; // Avoid truncation effects in permutation
    vec3 p = permute ( permute ( i . y + vec3 (0.0 , i1 .y , 1.0 ) )
    + i . x + vec3 (0.0 , i1 .x , 1.0 ) ) ;
    vec3 m = max (0.5 - vec3 ( dot ( x0 , x0 ) , dot ( x12 . xy , x12 . xy ) ,
    dot ( x12 . zw , x12 . zw ) ) , 0.0) ;
    m = m * m ; m = m * m ;
    // Gradients
    vec3 x = 2.0 * fract ( p * C . www ) - 1.0;
    vec3 h = abs ( x ) - 0.5;
    vec3 a0 = x - floor ( x + 0.5) ;
    // Normalise gradients implicitly by scaling m
    m *= 1.79284291400159 - 0.85373472095314 * ( a0 * a0 + h * h ) ;
    // Compute final noise value at P
    vec3 g ;
    g . x = a0 . x * x0 . x + h . x * x0 . y ;
    g . yz = a0 . yz * x12 . xz + h . yz * x12 . yw ;
    return 130.0 * dot (m , g ) ;
}


vec3 computeNormal( uint offset )
{
    vec2 size  = textureSize(IDK_SSBO_textures[offset+0], 0);
    vec2 uv = vsin_texcoords;

    float Lheight = textureOffset(IDK_SSBO_textures[offset+0], uv, ivec2(-1, 0)).r;
    float Rheight = textureOffset(IDK_SSBO_textures[offset+0], uv, ivec2(+1, 0)).r;
    float Uheight = textureOffset(IDK_SSBO_textures[offset+0], uv, ivec2(0, -1)).r;
    float Dheight = textureOffset(IDK_SSBO_textures[offset+0], uv, ivec2(0, +1)).r;

    vec3 left  = vec3(-1.0, Lheight, 0.0);
    vec3 right = vec3(+1.0, Rheight, 0.0);
    vec3 front = vec3(0.0, Uheight, -1.0);
    vec3 back  = vec3(0.0, Dheight, +1.0);

    return normalize(cross(right-left, front-back));
}


uniform uint un_draw_offset;

void main()
{
    draw_id = gl_DrawID + un_draw_offset;

    IDK_Camera camera = IDK_RenderData_GetCamera();

    const uint offset = IDK_SSBO_transform_offsets[draw_id];
    const mat4 model  = IDK_SSBO_transforms[offset + gl_InstanceID];

    uint texture_offset = IDK_SSBO_texture_offsets[draw_id];
    float height = texture(IDK_SSBO_textures[texture_offset+0], vsin_texcoords).r;

    vec4 position = vec4(vsin_pos, 1.0);
         position.y += height;
         position = model * position;

    vec4 tangent  = model * vec4(vsin_tangent, 0.0);


    mat3 NormalMatrix = inverse(transpose(mat3(model)));

    vec3 N = normalize(NormalMatrix * computeNormal(texture_offset));
    vec3 T = normalize(mat3(model) * normalize(vsin_tangent));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    B = normalize(B - dot(B, N) * N);


    fsin_fragpos   = position.xyz;
    fsin_normal    = N;
    fsin_tangent   = T;
    fsin_texcoords = 16.0 * vsin_texcoords;

    TBN  = mat3(T, B, N);
    TBNT = transpose(TBN);
    TBN_fragpos = TBNT * fsin_fragpos;
    TBN_viewpos = TBNT * camera.position.xyz;

    gl_Position = camera.P * camera.V * position;
}