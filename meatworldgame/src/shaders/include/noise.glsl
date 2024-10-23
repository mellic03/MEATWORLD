#ifndef IDK_NOISE
#define IDK_NOISE


#include "./bindings.glsl"


layout (std430, binding = IDK_BINDING_SSBO_Noise) readonly buffer IDK_SSBO_Noise
{
    sampler2D IDK_SSBO_WhiteNoise;
    sampler2D IDK_SSBO_BlueNoise;
    sampler2DArray IDK_SSBO_PerlinNoise;
    sampler2DArray IDK_SSBO_SuperPerlinNoise;
    sampler2DArray IDK_SSBO_VoronoiNoise;
    sampler2DArray IDK_SSBO_VeinNoise;
    sampler2DArray IDK_SSBO_CraterNoise;

    vec2 IDK_SSBO_NoiseDirections[16];
};


vec2 IDK_Vec2Noise( int i )
{
    return IDK_SSBO_NoiseDirections[i%16];
}


vec3 IDK_WhiteNoise( vec2 texcoord )
{
    return textureLod(IDK_SSBO_WhiteNoise, texcoord, 0.0).rgb;
}

vec3 IDK_WhiteNoiseTexel( ivec2 texel )
{
    return IDK_WhiteNoise(vec2(texel)/textureSize(IDK_SSBO_WhiteNoise, 0));
}


vec3 IDK_BlueNoise( vec2 texcoord )
{
    return textureLod(IDK_SSBO_BlueNoise, texcoord, 0.0).rgb;
}

vec3 IDK_BlueNoiseTexel( ivec2 texel )
{
    return IDK_BlueNoise(vec2(texel)/textureSize(IDK_SSBO_BlueNoise, 0));
}


vec2 IDK_BlueNoiseSize( int level )
{
    return textureSize(IDK_SSBO_BlueNoise, level);
}


vec3 IDK_PerlinNoise( vec2 texcoord, uint level )
{
    return textureLod(IDK_SSBO_PerlinNoise, vec3(texcoord, float(level%8)), 0.0).rgb;
}


vec3 IDK_PerlinNoiseTexel( ivec2 texel, uint level )
{
    vec2 uv = vec2(texel) / textureSize(IDK_SSBO_PerlinNoise, 0).xy;
    return IDK_PerlinNoise(uv, level);
}


vec3 IDK_PerlinNoiseOffset( vec2 texcoord, uint level, ivec2 delta )
{
    vec2 size = textureSize(IDK_SSBO_PerlinNoise, 0).xy;
    vec2 offset = vec2(delta) / size;
    return IDK_PerlinNoise(texcoord+offset, level);
}


float IDK_VoronoiNoise( vec2 texcoord, uint level )
{
    return textureLod(IDK_SSBO_VoronoiNoise, vec3(texcoord, float(level%8)), 0.0).r;
}


float IDK_VoronoiNoiseOffset( vec2 texcoord, uint level, ivec2 delta )
{
    vec2 size = textureSize(IDK_SSBO_VoronoiNoise, 0).xy;
    vec2 offset = vec2(delta) / size;
    return IDK_VoronoiNoise(texcoord+offset, level);
}


float IDK_VeinNoise( vec2 texcoord, uint level )
{
    return textureLod(IDK_SSBO_VeinNoise, vec3(texcoord, float(level)), 0.0).r;
}



#endif
