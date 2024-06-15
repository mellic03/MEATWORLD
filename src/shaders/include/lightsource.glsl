#define MAX_POINTLIGHTS 10
#define MAX_DIRLIGHTS   10
#define MAX_SPOTLIGHTS  10

#define EPSILON  0.000001
#define MIPLEVEL_DIFFUSE   2.0
#define NUM_DIRLIGHTS    0
#define NUM_POINTLIGHTS  0
#define NUM_SPOTLIGHTS   0
#define PI  3.14159265359


// vec3 fresnelSchlick( float cosTheta, vec3 F0 )
// {
//     return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
// }

// vec3 fresnelSchlickR( float cosTheta, vec3 F0, float roughness )
// {
//     return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
// }

// float NDF( float roughness, vec3 N, vec3 H )
// {
//     float a = roughness*roughness;
//     float a2 = a*a;
//     float NdotH = max(dot(N, H), 0.0);
//     float NdotH2 = NdotH*NdotH;

//     float nom   = a2;
//     float denom = (NdotH2 * (a2 - 1.0) + 1.0);
//     denom = PI * denom * denom;

//     return nom / denom;
// }

// float GeometrySchlickGGX(float NdotV, float roughness)
// {
//     float r = (roughness + 1.0);
//     float k = (r*r) / 8.0;
//     float nom   = NdotV;
//     float denom = NdotV * (1.0 - k) + k;
//     return nom / denom;
// }

// float GSF( float roughness, vec3 N, vec3 V, vec3 L )
// {
//     float NdotV = max(dot(N, V), 0.0);
//     float NdotL = max(dot(N, L), 0.0);
//     float ggx2 = GeometrySchlickGGX(NdotV, roughness);
//     float ggx1 = GeometrySchlickGGX(NdotL, roughness);

//     return ggx1 * ggx2;
// }




// Point Lights
// ---------------------------------------------------------------------------------------------
struct PointLight
{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 attenuation;
};

layout (std140, binding = 3) uniform UBO_pointlights
{
    PointLight  ubo_pointlights[MAX_POINTLIGHTS];
};
// ---------------------------------------------------------------------------------------------



// Directional Lights
// ---------------------------------------------------------------------------------------------
struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};

layout (std140, binding = 5) uniform UBO_dirlights
{
    DirLight    un_dirlights[MAX_DIRLIGHTS];
    mat4        un_cascade_matrices[4];
};

uniform sampler2DArrayShadow   un_dirlight_depthmap;
uniform vec4                   un_cascade_depths;

#define KERNEL_HW 2
#define BLEND_DIST 1.0
#define DIRLIGHT_BIAS 0.0005


float sampleDepthMap( int layer, vec3 uv, float bias )
{
    vec2 texelSize = 0.5 / textureSize(un_dirlight_depthmap, 0).xy;

    float shadow = 0.0;

    for(int x = -KERNEL_HW; x <= KERNEL_HW; ++x)
    {
        for(int y = -KERNEL_HW; y <= KERNEL_HW; ++y)
        {
            vec2 sample_uv    = uv.xy + vec2(x, y) * texelSize;
            vec4 sample_coord = vec4(sample_uv, float(layer), uv.z - bias);

            shadow += texture(un_dirlight_depthmap, sample_coord); 
        }
    }

    return shadow / ((2*KERNEL_HW+1)*(2*KERNEL_HW+1));
}


// float dirlight_shadow( int idx, mat4 view_matrix, vec3 position, vec3 N )
// {
//     DirLight light = un_dirlights[idx];
//     vec3 L = normalize(-light.direction.xyz);

//     vec3  fragpos_viewspace = (view_matrix * vec4(position, 1.0)).xyz;
//     float frag_depth        = abs(fragpos_viewspace.z);

//     vec4 res   = step(un_cascade_depths, vec4(frag_depth));
//     int  layer = int(res.x + res.y + res.z + res.w);

//     vec4 fragpos_lightspace = un_cascade_matrices[layer] * vec4(position, 1.0);
//     vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;
//     projCoords = projCoords * 0.5 + 0.5;

//     float bias   = 0.0005 * max(dot(N, L), 0.00001);
//     // float bias = DIRLIGHT_BIAS * max(dot(N, L), 0.0);
//     float shadow = sampleDepthMap(layer, projCoords, bias);

//     return shadow;
// }


// float dirlight_shadow_NoSlopeBias( int idx, mat4 view_matrix, vec3 position )
// {
//     DirLight light = un_dirlights[idx];
//     vec3 L = normalize(-light.direction.xyz);

//     vec3  fragpos_viewspace = (view_matrix * vec4(position, 1.0)).xyz;
//     float frag_depth        = abs(fragpos_viewspace.z);

//     vec4 res   = step(un_cascade_depths, vec4(frag_depth));
//     int  layer = int(res.x + res.y + res.z + res.w);

//     vec4 fragpos_lightspace = un_cascade_matrices[layer] * vec4(position, 1.0);
//     vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;
//     projCoords = projCoords * 0.5 + 0.5;

//     float bias   = DIRLIGHT_BIAS;
//     float shadow = sampleDepthMap(layer, projCoords, bias);

//     return shadow;
// }





// float sampleDepthMap_2( sampler2DShadow depth_map, vec3 uv, float bias )
// {
//     vec2 texelSize = 0.5 / textureSize(depth_map, 0).xy;

//     float shadow = 0.0;

//     for(int x = -KERNEL_HW; x <= KERNEL_HW; ++x)
//     {
//         for(int y = -KERNEL_HW; y <= KERNEL_HW; ++y)
//         {
//             vec2 sample_uv    = uv.xy + vec2(x, y) * texelSize;
//             // vec2 sample_uv    = uv.xy;
//             vec3 sample_coord = vec3(sample_uv, uv.z - bias);

//             shadow += texture(depth_map, sample_coord); 
//         }
//     }

//     // return shadow;
//     return shadow / ((2*KERNEL_HW+1)*(2*KERNEL_HW+1));
// }



// float dirlight_shadow_2( int idx, sampler2DShadow depth_map, mat4 view_matrix, mat4 light_matrix,
//                          vec3 position, vec3 N )
// {
//     DirLight light = un_dirlights[idx];
//     vec3 L = normalize(-light.direction.xyz);

//     vec3  fragpos_viewspace  = (view_matrix * vec4(position, 1.0)).xyz;
//     float frag_depth         = abs(fragpos_viewspace.z);
//     vec4  fragpos_lightspace = light_matrix * vec4(position, 1.0);

//     vec3  projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;
//           projCoords = projCoords * 0.5 + 0.5;

//     float bias   = 0.02 * max(dot(N, L), 0.0001);
//     float shadow = sampleDepthMap_2(depth_map, projCoords, bias);

//     return clamp(shadow, 0.0, 1.0);
// }


vec3 dirlight_contribution( int idx, vec3 position, vec3 F0, vec3 N, vec3 V, vec3 R,
                            vec3 albedo, float metallic, float roughness, vec3 brdf )
{
    DirLight light       = un_dirlights[idx];
    vec3  light_diffuse  = light.diffuse.xyz;
    float light_strength = light.diffuse.w;

    const vec3 L = normalize(-light.direction.xyz);
    const vec3 H = normalize(V + L);
    float NdotL  = max(dot(N, L), 0.0);

    float ndf = NDF(roughness, N, H);
    float G = GSF(roughness, N, V, L);
    vec3  F = fresnelSchlickR(max(dot(H, V), 0.0), F0, roughness);

    vec3  numerator   = ndf * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + EPSILON;
    vec3  specular    = numerator / denominator;

    vec3 Ks = F;
    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);

    return (Kd * albedo/PI + (specular * brdf)) * light_strength * light_diffuse * NdotL;
}


vec3 dirlight_contribution_no_pbr( int idx, vec3 position, vec3 N, vec3 V, vec3 R, vec3 albedo )
{
    DirLight light       = un_dirlights[idx];
    vec3  light_diffuse  = light.diffuse.xyz;
    float light_strength = light.diffuse.w;

    const vec3 L = normalize(-light.direction.xyz);
    const vec3 H = normalize(V + L);
    float NdotL  = max(dot(N, L), 0.0);

    float spec = pow(max(dot(N, H), 0.0), 4.0);

    return albedo * light_diffuse * spec * NdotL;
}
// ---------------------------------------------------------------------------------------------
