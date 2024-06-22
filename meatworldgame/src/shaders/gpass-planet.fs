#version 460 core

#extension GL_GOOGLE_include_directive: require
#extension GL_ARB_bindless_texture: require

#include "./include/storage.glsl"
#include "./include/util.glsl"

layout (location = 0) out vec4 fsout_albedo;
layout (location = 1) out uvec3 fsout_normal;
layout (location = 2) out vec4 fsout_pbr;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec3 fsin_tangent;
in vec2 fsin_texcoords;
flat in int material_id;


in vec3 TBN_viewpos;
in vec3 TBN_fragpos;
in mat3 TBN;
in mat3 TBNT;



void main()
{
    vec2 texcoords = fsin_texcoords;

    vec4  albedo = texture(IDK_SSBO_materials[material_id][0], texcoords).rgba;
    vec3  normal = texture(IDK_SSBO_materials[material_id][1], texcoords).xyz * 2.0 - 1.0;
    vec3  ao_r_m = texture(IDK_SSBO_materials[material_id][2], texcoords).rgb;
    float noidea = texture(IDK_SSBO_materials[material_id][3], texcoords).r;
    float emissv = texture(IDK_SSBO_materials[material_id][4], texcoords).r;

    float ao        = ao_r_m.r;
    float roughness = ao_r_m.g;
    float metallic  = ao_r_m.b;

    vec3 N = normalize(fsin_normal);
        //  N = normalize(mix(N, normalize(fsin_normal), 0.5));

    fsout_albedo = vec4(albedo.rgb, 1.0);
    fsout_normal = IDK_PackNormal(N);
    fsout_pbr    = vec4(roughness, metallic, ao, emissv);
}
