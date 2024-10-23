#version 460 core

#extension GL_GOOGLE_include_directive: require
#include "./include/storage.glsl"
#include "./include/util.glsl"

vec4 triplanar( uint idx, vec3 pos, vec3 N )
{
    vec4 result = vec4(0.0);

    vec3 W = abs(N);
         W /= (W.x + W.y + W.z);

    result += W.z * texture(IDK_SSBO_textures[idx], pos.xy);
    result += W.x * texture(IDK_SSBO_textures[idx], pos.yz);
    result += W.y * texture(IDK_SSBO_textures[idx], pos.xz);

    return result;
}



layout (location = 0) out vec4 fsout_albedo;
layout (location = 1) out vec3 fsout_normal;
layout (location = 2) out vec4 fsout_pbr;
// layout (location = 3) out vec2 fsout_vel;



in FS_in
{
    vec3 fragpos;
    vec3 normal;
    vec3 tangent;
    vec2 texcoord;
    flat uint drawID;

    vec4 pos_curr;
    vec4 pos_prev;
} fsin;


in vec3 TBN_viewpos;
in vec3 TBN_fragpos;
in mat3 TBN;
in mat3 TBNT;


void main()
{
    vec2 texcoords = fsin.texcoord;
    uint offset    = IDK_SSBO_texture_offsets[fsin.drawID];

    vec4  albedo = texture(IDK_SSBO_textures[offset+0], texcoords).rgba;
    vec3  normal = texture(IDK_SSBO_textures[offset+1], texcoords).xyz * 2.0 - 1.0;
    vec3  ao_r_m = texture(IDK_SSBO_textures[offset+2], texcoords).rgb;
    float noidea = texture(IDK_SSBO_textures[offset+3], texcoords).r;
    float emissv = texture(IDK_SSBO_textures[offset+4], texcoords).r;

    float ao        = ao_r_m.r;
    float roughness = ao_r_m.g;
    float metallic  = ao_r_m.b;

    // vec3 N = normalize(TBN * normalize(normal)); // normalize(fsin.normal);
        //  N = normalize(mix(N, normalize(fsin.normal), 0.5));

    fsout_albedo = vec4(albedo.rgb, 1.0);
    fsout_normal = TBN * normalize(normal);
    fsout_pbr    = vec4(roughness, metallic, ao, emissv);


    // vec2 curr_uv = (fsin.pos_curr.xy / fsin.pos_curr.w) * 0.5 + 0.5;
    // vec2 prev_uv = (fsin.pos_prev.xy / fsin.pos_prev.w) * 0.5 + 0.5;
    // fsout_vel = curr_uv.xy - prev_uv.xy;

}


