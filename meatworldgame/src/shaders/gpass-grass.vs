#version 460 core

#extension GL_GOOGLE_include_directive: require
#include "./include/storage.glsl"


layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec2 fsin_texcoords;
flat out uint drawID;

uniform uint un_draw_offset;


void main()
{
    drawID = gl_DrawID + un_draw_offset;

    IDK_Camera camera = IDK_UBO_cameras[0];

    const uint offset = IDK_SSBO_transform_offsets[drawID];
    const mat4 model  = IDK_SSBO_transforms[offset + gl_InstanceID];

    vec4 position = model * vec4(vsin_pos,     1.0);
    vec4 normal   = model * vec4(vsin_normal,  0.0);

    fsin_fragpos   = position.xyz;
    fsin_normal    = normalize(position.xyz);
    fsin_texcoords = vsin_texcoords;

    gl_Position = (camera.P * camera.V) * position;
}