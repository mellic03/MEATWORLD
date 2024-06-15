#version 460 core

#extension GL_GOOGLE_include_directive: require
#extension GL_ARB_bindless_texture: require

#include "./include/SSBO_indirect.glsl"
#include "./include/util.glsl"

layout (location = 0) out vec4 fsout_albedo;
layout (location = 1) out vec3 fsout_normal;
layout (location = 2) out vec4 fsout_pbr;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec3 fsin_tangent;
in vec2 fsin_texcoords;
flat in int draw_id;


in vec3 TBN_viewpos;
in vec3 TBN_fragpos;
in mat3 TBN;
in mat3 TBNT;



void main()
{
    vec2 texcoords = fsin_texcoords;

    uint  offset = un_IndirectDrawData.texture_offsets[draw_id];
    float height = texture(un_IndirectDrawData.textures[offset+0], texcoords).r;

    // vec3 N = normalize(TBN * normalize(normal)); // normalize(fsin_normal);
        //  N = normalize(mix(N, normalize(fsin_normal), 0.5));

    fsout_albedo = vec4(vec3(height), 1.0);
    fsout_normal = fsin_normal;
    fsout_pbr    = vec4(0.9, 0.05, 1.0, 0.0);
}
