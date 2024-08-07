#include "./bindings.glsl"


layout (std430, binding = IDK_BINDING_SSBO_Noise) readonly buffer IDK_SSBO_Noise
{
    sampler2D IDK_SSBO_WhiteNoise;
    sampler2D IDK_SSBO_BlueNoise;
    sampler2DArray IDK_SSBO_PerlinNoise;
};


vec3 IDK_WhiteNoise( vec2 texcoord )
{
    return textureLod(IDK_SSBO_WhiteNoise, texcoord, 0.0).rgb;
}


vec3 IDK_BlueNoise( vec2 texcoord )
{
    return textureLod(IDK_SSBO_BlueNoise, texcoord, 0.0).rgb;
}


vec3 IDK_PerlinNoise( vec2 texcoord, uint level )
{
    level = clamp(level, 0, 5);
    return textureLod(IDK_SSBO_PerlinNoise, vec3(texcoord, float(level)), 0.0).rgb;
}


vec3 IDK_PerlinNoiseOffset( vec2 texcoord, uint level, ivec2 delta )
{
    vec2 size = textureSize(IDK_SSBO_PerlinNoise, 0).xy;
    vec2 offset = vec2(delta) / size;
    return IDK_PerlinNoise(texcoord+offset, level);
}
