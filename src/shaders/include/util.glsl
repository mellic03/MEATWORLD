

vec3 IDK_WorldToUV( vec3 world_position, mat4 PV )
{
    vec4 proj = PV * vec4(world_position, 1.0);
    proj.xy = (proj.xy / proj.w) * 0.5 + 0.5;

    return proj.xyz;
}

ivec2 IDK_WorldToTexel( vec3 world_position, mat4 PV, vec2 image_size )
{
    vec2 uv = IDK_WorldToUV(world_position, PV).xy;
    return ivec2(uv * image_size);
}


vec3 IDK_UVToWorld( vec2 uv, mat4 view_matrix )
{
    return vec3(0.0);
}

vec3 IDK_TexelToWorld( ivec2 texel, mat4 view_matrix )
{

    return vec3(0.0);
}


float IDK_DepthAtTexel( ivec2 texel, mat4 PV, image2D image )
{
    float frag_depth = (PV * imageLoad(image, texel)).z;
    return frag_depth;
}



vec3 IDK_WorldFromDepth( sampler2D depth, vec2 texcoords, mat4 P, mat4 V )
{
    float z = textureLod(depth, texcoords, 0.0).r * 2.0 - 1.0;

    vec4 pos = vec4(texcoords * 2.0 - 1.0, z, 1.0);
         pos = inverse(P) * pos;
         pos /= pos.w;
         pos = inverse(V) * pos;
    
    return pos.xyz;
}




vec3 IDK_PackNormal( vec3 N )
{
    return (N * 0.5 + 0.5);
}


vec3 IDK_UnpackNormal( vec3 N )
{
    return normalize(N * 2.0 - 1.0);
}


