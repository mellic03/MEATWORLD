#version 460 core

#extension GL_GOOGLE_include_directive: require
#extension GL_ARB_bindless_texture: require

#include "include/storage.glsl"
#include "include/storage.glsl"

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec3 fsin_tangent;
out vec2 fsin_texcoords;
flat out int material_id;

out vec3 TBN_viewpos;
out vec3 TBN_fragpos;
out mat3 TBN;
out mat3 TBNT;


void main()
{
    material_id = gl_DrawID;

    IDK_Camera camera = IDK_RenderData_GetCamera();

    const uint offset = IDK_SSBO_offsets[gl_DrawID];
    const mat4 model  = IDK_SSBO_transforms[offset + gl_InstanceID];

    vec4 position = model * vec4(vsin_pos,     1.0);
    vec4 normal   = model * vec4(vsin_normal,  0.0);
    vec4 tangent  = model * vec4(vsin_tangent, 0.0);

    vec3 N = normalize(mat3(model) * normalize(vsin_normal));
    vec3 T = normalize(mat3(model) * normalize(vsin_tangent));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    B = normalize(B - dot(B, N) * N);


    fsin_fragpos   = position.xyz;
    fsin_normal    = N;
    fsin_tangent   = T;
    fsin_texcoords = vsin_texcoords;

    TBN  = mat3(T, B, N);
    TBNT = transpose(TBN);
    TBN_fragpos = TBNT * fsin_fragpos;
    TBN_viewpos = TBNT * camera.position.xyz;

    gl_Position = (camera.P * camera.V) * position;
}