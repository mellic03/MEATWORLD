#version 460 core

#extension GL_GOOGLE_include_directive: require
#extension GL_ARB_bindless_texture: require

#include "./include/SSBO_indirect.glsl"

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

uniform mat4 un_lightspacematrix;


void main()
{
    const uint offset = un_IndirectDrawData.transform_offsets[gl_DrawID];
    const mat4 model  = un_IndirectDrawData.transforms[offset + gl_InstanceID];

    uint texture_offset = un_IndirectDrawData.texture_offsets[gl_DrawID];
    float height = texture(un_IndirectDrawData.textures[texture_offset+0], vsin_texcoords).r;

    vec4 position = vec4(vsin_pos, 1.0);
         position.y += height;
         position = model * position;


    gl_Position = un_lightspacematrix * position;
}  



