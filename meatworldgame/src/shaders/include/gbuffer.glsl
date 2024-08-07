#include "./bindings.glsl"

layout (std140, binding = IDK_BINDING_UBO_GBuffer) uniform IDK_UBO_Lightsources
{
    sampler2D       GBuffer_prev_diff;
    sampler2D       GBuffer_prev_norm;
    sampler2D       GBuffer_prev_aorm;
    sampler2D       GBuffer_prev_depth;

    sampler2D       GBuffer_prev_ssao;
    sampler2D       GBuffer_prev_vol;

};

