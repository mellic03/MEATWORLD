#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec2 fsin_texcoords;

out FS_IN
{
    vec3 fragpos;
    vec2 texcoords;
} vsout;


uniform mat4 un_projee;


void main()
{
    vec4 projected = un_projee * vec4(vsin_pos, 1.0);

    vsout.fragpos   = projected.xyz;
    vsout.texcoords = fsin_texcoords;

    gl_Position = projected;
}