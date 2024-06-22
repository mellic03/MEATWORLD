#version 460 core

#extension GL_GOOGLE_include_directive: require
#include "./include/storage.glsl"


layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

uniform uint un_light_id;
uniform uint un_draw_offset;



void main()
{
    IDK_Dirlight light = IDK_UBO_dirlights[un_light_id];

    const uint drawID = gl_DrawID + un_draw_offset;
    const uint offset = IDK_SSBO_transform_offsets[drawID];
    const mat4 model  = IDK_SSBO_transforms[offset + gl_InstanceID];

    uint texture_offset = IDK_SSBO_texture_offsets[drawID];
    float height = texture(IDK_SSBO_textures[texture_offset+0], vsin_texcoords).r;

    vec4 position = vec4(vsin_pos, 1.0);
         position.y += height;

    gl_Position = light.transform * model * position;
}  
