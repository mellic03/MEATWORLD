#version 460 core

#extension GL_GOOGLE_include_directive: require
#extension GL_ARB_bindless_texture: require

#include "./include/SSBO_indirect.glsl"
#include "./include/UBOs.glsl"

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec3 fsin_tangent;
out vec2 fsin_texcoords;
flat out int draw_id;

out vec3 TBN_viewpos;
out vec3 TBN_fragpos;
out mat3 TBN;
out mat3 TBNT;



vec3 computeNormal( uint offset )
{
    vec2 size  = textureSize(un_IndirectDrawData.textures[offset+0], 0);
    vec2 uv = vsin_texcoords;

    float Lheight = textureOffset(un_IndirectDrawData.textures[offset+0], uv, ivec2(-1, 0)).r;
    float Rheight = textureOffset(un_IndirectDrawData.textures[offset+0], uv, ivec2(+1, 0)).r;
    float Uheight = textureOffset(un_IndirectDrawData.textures[offset+0], uv, ivec2(0, -1)).r;
    float Dheight = textureOffset(un_IndirectDrawData.textures[offset+0], uv, ivec2(0, +1)).r;

    vec3 left  = vec3(-1.0, Lheight, 0.0);
    vec3 right = vec3(+1.0, Rheight, 0.0);
    vec3 front = vec3(0.0, Uheight, -1.0);
    vec3 back  = vec3(0.0, Dheight, +1.0);

     return normalize(cross(right-left, front-back));

//     vec3 N = normalize(0.25 * vec3(2.0 * (Rheight-Lheight), -4.0, 2.0*(Dheight-Uheight)));
//          N.y *= -1.0;
    
//     return N;

    // return normalize(vec3(Lheight-Rheight, 2.0/30.0, Uheight-Dheight));
    // return normalize(cross(xn-xp, zn-zp));
}



void main()
{
    draw_id = gl_DrawID;

    IDK_Camera camera = IDK_RenderData_GetCamera();

    const uint offset = un_IndirectDrawData.transform_offsets[gl_DrawID];
    const mat4 model  = un_IndirectDrawData.transforms[offset + gl_InstanceID];

    uint texture_offset = un_IndirectDrawData.texture_offsets[gl_DrawID];
    float height = texture(un_IndirectDrawData.textures[texture_offset+0], vsin_texcoords).r;

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

    gl_Position = camera.PV * position;
}