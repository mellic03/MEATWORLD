
#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in FS_IN
{
    vec3 fragpos;
    vec2 texcoords;

} fsin;


struct DenseVoxel
{
    vec3 color; int count;
};

layout (std430, binding = 4) buffer DENSE_DATA
{
    DenseVoxel voxels[];
};


uniform sampler2D un_albedo_texture;
uniform sampler2D un_specular_texture;
uniform sampler2D un_normal_texture;
uniform sampler2D un_reflection_texture;

uniform float un_mesh_width;
uniform float un_octree_span;


void main()
{

    fsout_frag_color = vec4(0.0, 0.0, 1.0, 1.0);
}



