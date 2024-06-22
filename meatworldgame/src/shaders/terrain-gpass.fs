#version 460 core

#extension GL_GOOGLE_include_directive: require
#extension GL_ARB_bindless_texture: require

#include "./include/storage.glsl"
#include "./include/util.glsl"

layout (location = 0) out vec4 fsout_albedo;
layout (location = 1) out vec3 fsout_normal;
layout (location = 2) out vec4 fsout_pbr;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec3 fsin_tangent;
in vec2 fsin_texcoords;
flat in uint draw_id;


in vec3 TBN_viewpos;
in vec3 TBN_fragpos;
in mat3 TBN;
in mat3 TBNT;



void main()
{
    vec2 texcoords = fsin_texcoords;

    uint  offset = IDK_SSBO_texture_offsets[draw_id];
    float height = texture(IDK_SSBO_textures[offset+0], texcoords).r;
    vec3  albedo = texture(IDK_SSBO_textures[offset+1], texcoords).rgb;
    vec3  normal = texture(IDK_SSBO_textures[offset+2], texcoords).rgb;
    vec3  ao_r_m = texture(IDK_SSBO_textures[offset+3], texcoords).rgb;

    vec3  albedo2 = texture(IDK_SSBO_textures[offset+4], 4.0*texcoords).rgb;
    vec3  normal2 = texture(IDK_SSBO_textures[offset+5], 4.0*texcoords).rgb;
    vec3  ao_r_m2 = texture(IDK_SSBO_textures[offset+6], 4.0*texcoords).rgb;


    float ao        = ao_r_m.r;
    float roughness = ao_r_m.g;
    float metallic  = ao_r_m.b;

    float alpha = max(dot(fsin_normal, vec3(0.0, 1.0, 0.0)), 0.0);
          alpha = 1.0 - alpha;
    
          alpha = smoothstep(0.0, 0.1, alpha);

    vec3 A = mix(albedo, albedo2, alpha);

    // vec3 N = normalize(TBN * normalize(normal)); // normalize(fsin_normal);
        //  N = normalize(mix(N, normalize(fsin_normal), 0.5));

    fsout_albedo = vec4(A, 1.0);
    fsout_normal = TBN * normalize(normal);
    fsout_pbr    = vec4(0.8, 0.0, ao, 0.0);
}
