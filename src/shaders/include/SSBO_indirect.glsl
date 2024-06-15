
#define idk_indirect_draw_TEXTURES_PER_MATERIAL 5

#define idk_indirect_draw_MAX_TEXTURES  2048
#define idk_indirect_draw_MAX_TRANSFORMS 2048
#define idk_indirect_draw_MAX_DRAW_CALLS 512


struct IDK_IndirectDrawData
{
    sampler2D   textures          [idk_indirect_draw_MAX_TEXTURES];
    mat4        transforms        [idk_indirect_draw_MAX_TRANSFORMS];
    uint        transform_offsets [idk_indirect_draw_MAX_DRAW_CALLS];
    uint        texture_offsets   [idk_indirect_draw_MAX_DRAW_CALLS];
};


layout (std430, binding = 0) buffer SSBO_IndirectDrawData
{
    IDK_IndirectDrawData un_IndirectDrawData;
};

