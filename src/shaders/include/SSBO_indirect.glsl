
#define idk_indirect_draw_TEXTURES_PER_MATERIAL 5

#define idk_indirect_draw_MAX_MATERIALS  2048
#define idk_indirect_draw_MAX_TRANSFORMS 2048
#define idk_indirect_draw_MAX_DRAW_CALLS 512


struct IDK_IndirectDrawData
{
    sampler2D   materials       [idk_indirect_draw_MAX_MATERIALS][idk_indirect_draw_TEXTURES_PER_MATERIAL];
    sampler2D   user_materials  [idk_indirect_draw_MAX_MATERIALS][idk_indirect_draw_TEXTURES_PER_MATERIAL];
    mat4        transforms      [idk_indirect_draw_MAX_TRANSFORMS];
    uint        offsets         [idk_indirect_draw_MAX_DRAW_CALLS];
};


layout (std430, binding = 0) buffer SSBO_IndirectDrawData
{
    IDK_IndirectDrawData un_IndirectDrawData;
};

