#version 460 core

#extension GL_GOOGLE_include_directive: require
#include "./include/storage.glsl"


layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;


out FS_in
{
    vec3 fragpos;
    vec3 normal;
    vec3 tangent;
    vec2 texcoord;
    flat uint drawID;

    vec4 pos_curr;
    vec4 pos_prev;
} vsout;

out vec3 TBN_viewpos;
out vec3 TBN_fragpos;
out mat3 TBN;
out mat3 TBNT;

uniform uint un_draw_offset;


void main()
{
    vsout.drawID = gl_DrawID + un_draw_offset;

    IDK_Camera camera = IDK_UBO_cameras[0];

    const uint offset = IDK_SSBO_transform_offsets[vsout.drawID];
    const mat4 model  = IDK_SSBO_transforms[offset + gl_InstanceID];
    const mat4 prev_T = IDK_SSBO_prev_transforms[offset + gl_InstanceID];

    vec4 position = model * vec4(vsin_pos,     1.0);
    vec4 normal   = model * vec4(vsin_normal,  0.0);
    vec4 tangent  = model * vec4(vsin_tangent, 0.0);


    vec3 N = normalize(mat3(model) * normalize(vsin_normal));
    vec3 T = normalize(mat3(model) * normalize(vsin_tangent));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    B = normalize(B - dot(B, N) * N);


    vsout.fragpos   = position.xyz;
    vsout.normal    = N;
    vsout.tangent   = T;

    vsout.pos_curr = camera.P * camera.V * model * vec4(vsin_pos, 1.0);
    vsout.pos_prev = camera.prev_P * camera.prev_V * prev_T * vec4(vsin_pos, 1.0);

    vsout.texcoord  = vec2(length(vec3(model[0]))*vsin_pos.x + length(vec3(model[2]))*vsin_pos.z, vsin_pos.y);
    vsout.texcoord *= 0.25 * vec2(1.0, length(vec3(model[1])));

    TBN  = mat3(T, B, N);
    TBNT = transpose(TBN);
    TBN_fragpos = TBNT * vsout.fragpos;
    TBN_viewpos = TBNT * camera.position.xyz;

    gl_Position = camera.P * camera.V * position;
}