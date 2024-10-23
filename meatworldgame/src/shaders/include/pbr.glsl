#define PBR_PI  3.14159265359
#define PBR_EPSILON 0.0001


#define PBR_DOT(u, v) max(dot(u, v), 0.001)
// #define PBR_DOT(u, v) dot(u, v) * 0.5 + 0.5


vec3 fresnelSchlick( float cosTheta, vec3 F0 )
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


vec3 fresnelSchlickR( float cosTheta, vec3 F0, float roughness )
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


float fresnelDisney( vec3 N, vec3 V, vec3 L, float roughness )
{
    vec3 H  = normalize(V + L);

    float NdotH = max(dot(N, H), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float f90 = 0.5 + 2.0*(roughness * NdotH*NdotH);
    float f0  = 1.0;
    float fd  = mix(f0, f90, NdotL) * mix(f0, f90, NdotV);

    return fd;
}


float NDF( float roughness, vec3 N, vec3 H )
{
    float a = roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);

    float denom = NdotH*NdotH * (a2 - 1.0) + 1.0;
          denom = PBR_PI * denom*denom;

    return a2 / denom;
}


float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GGX( float cosTheta, float k )
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}


float GSF( float roughness, vec3 N, vec3 V, vec3 L )
{
    float NdotV = max(dot(N, V), PBR_EPSILON);
    float NdotL = max(dot(N, L), PBR_EPSILON);

    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;

    // float k = (roughness * roughness) / 8.0;
    // float a = NdotV * (1.0 - k) + k;
    // float b = NdotL * (1.0 - k) + k;

    // return GGX(NdotV, k) * GGX(NdotL, k);
    // return 0.25 / (a*b + PBR_EPSILON);

    // return fresnelDisney(N, V, L, roughness);
}




struct IDK_PBRSurfaceData
{
    vec3 position;

    vec3  albedo;
    float alpha;

    float roughness, metallic, ao, emission;

    vec3  N, V, R;
    float NdotV;
    vec3  F0, F, Ks, Kd;
    vec3  brdf;

};



IDK_PBRSurfaceData
IDK_PBRSurfaceData_load( IDK_Camera camera, vec2 texcoord, sampler2D depth_tex, sampler2D albedo_tex,
                         sampler2D normal_tex, sampler2D PBR_tex, sampler2D BRDF_LUT )
{
    IDK_PBRSurfaceData data;

    vec4  albedo_a = texture(albedo_tex, texcoord);
    vec3  albedo   = albedo_a.rgb;
    float alpha    = albedo_a.a;

    vec3  worldpos = IDK_WorldFromDepth(depth_tex, texcoord, camera.P, camera.V);
    vec3  normal   = texture(normal_tex, texcoord).xyz;

    vec4  texture_pbr = texture(PBR_tex, texcoord);
    float roughness   = clamp(texture_pbr.r, 0.0, 1.0);
    float metallic    = clamp(texture_pbr.g, 0.0, 1.0);
    float ao          = clamp(texture_pbr.b, 0.0, 1.0);
    float emission    = clamp(texture_pbr.a, 0.0, 1.0);

    vec3  N     = normal;
    vec3  V     = normalize(camera.position.xyz - worldpos);
    vec3  R     = reflect(-V, N); 
    vec3  F0    = clamp(mix(vec3(0.04), albedo, metallic), 0.0, 1.0);
    float NdotV = max(dot(N, V), PBR_EPSILON);
    vec3  F     = fresnelSchlickR(NdotV, F0, roughness);
    vec3  Ks    = F;
    vec3  Kd    = (vec3(1.0) - Ks) * (1.0 - metallic);
    vec2  BRDF  = texture(BRDF_LUT, vec2(NdotV+1.0/128.0, roughness+1.0/128.0)).rg;
    vec3  brdf  = (Ks * BRDF.x + BRDF.y);

    data.position   = worldpos;

    data.albedo     = albedo;
    data.alpha      = alpha;

    data.roughness  = roughness;
    data.metallic   = metallic;
    data.ao         = ao;
    data.emission   = emission;

    data.N      = N;
    data.V      = V;
    data.R      = R;
    data.F0     = F0;
    data.NdotV  = NdotV;
    data.F      = F;
    data.Ks     = Ks;
    data.Kd     = Kd;
    data.brdf   = brdf;

    return data;
}



IDK_PBRSurfaceData
IDK_PBRSurfaceData_load2( IDK_Camera camera, vec2 texcoord, sampler2D depth_tex, sampler2D depth_tex2,
                          sampler2D albedo_tex,
                          sampler2D normal_tex, sampler2D PBR_tex, sampler2D BRDF_LUT )
{
    IDK_PBRSurfaceData data;

    vec4  albedo_a = texture(albedo_tex, texcoord);
    vec3  albedo   = albedo_a.rgb;
    float alpha    = albedo_a.a;


    float depth = texture(depth_tex, texcoord).r;
          depth = min(depth, texture(depth_tex2, texcoord).r);

    vec3  worldpos = IDK_WorldFromDepthSample(depth, texcoord, camera.P, camera.V);
    vec3  normal   = texture(normal_tex, texcoord).xyz;

    vec4  texture_pbr = texture(PBR_tex, texcoord);
    float roughness   = clamp(texture_pbr.r, 0.0, 1.0);
    float metallic    = clamp(texture_pbr.g, 0.0, 1.0);
    float ao          = clamp(texture_pbr.b, 0.0, 1.0);
    float emission    = clamp(texture_pbr.a, 0.0, 1.0);

    vec3  N     = normal;
    vec3  V     = normalize(camera.position.xyz - worldpos);
    vec3  R     = reflect(-V, N); 
    vec3  F0    = clamp(mix(vec3(0.04), albedo, metallic), 0.0, 1.0);
    float NdotV = max(dot(N, V), 0.01);
    vec3  F     = fresnelSchlickR(NdotV, F0, roughness);
    vec3  Ks    = F;
    vec3  Kd    = (vec3(1.0) - Ks) * (1.0 - metallic);
    vec2  BRDF  = textureLod(BRDF_LUT, vec2(NdotV, roughness), 0.0).rg;
    vec3  brdf  = (Ks * BRDF.x + BRDF.y);


    data.position   = worldpos;

    data.albedo     = albedo;
    data.alpha      = alpha;

    data.roughness  = roughness;
    data.metallic   = metallic;
    data.ao         = ao;
    data.emission   = emission;

    data.N      = N;
    data.V      = V;
    data.R      = R;
    data.F0     = F0;
    data.NdotV  = NdotV;
    data.F      = F;
    data.Ks     = Ks;
    data.Kd     = Kd;
    data.brdf   = brdf;

    return data;
}





vec3 IDK_PBR_Pointlight( IDK_Pointlight light, IDK_PBRSurfaceData surface, vec3 fragpos )
{
    vec3  light_position = light.position.xyz;
    vec3  light_diffuse  = light.diffuse.xyz;

    const vec3 L = normalize(light_position - fragpos);
    const vec3 H  = normalize(surface.V + L);

    float ndf   = NDF(surface.roughness, surface.N, H);
    float G     = GSF(surface.roughness, surface.N, surface.V, L);
    float NdotL = PBR_DOT(surface.N, L);

    vec3  numerator   = ndf * G * surface.F;
    float denominator = 4.0 * surface.NdotV * NdotL + PBR_EPSILON;
    vec3  specular    = light.diffuse.a * (numerator / denominator); // Can zero-out specular.

    float d = distance(light_position, fragpos);
    float attenuation = d / light.radius;
          attenuation = 1.0 - clamp(attenuation, 0.0, 1.0);

    return attenuation * ((surface.Kd * surface.albedo) / PBR_PI + specular) * light_diffuse * NdotL;
}


vec3 IDK_PBR_Spotlight( IDK_Spotlight light, IDK_PBRSurfaceData surface, vec3 fragpos )
{
    vec3  light_position = light.position.xyz;
    vec3  light_diffuse  = light.diffuse.rgb;

    const vec3 L = normalize(light_position - fragpos);
    const vec3 H  = normalize(surface.V + L);

    float theta    = acos(dot(light.direction.xyz, -L));
    float strength = 1.0 - clamp(theta / light.angle[0], 0.0, 1.0);

    if (strength <= 0.0)
    {
        return vec3(0.0);
    }


    float ndf   = NDF(surface.roughness, surface.N, H);
    float G     = GSF(surface.roughness, surface.N, surface.V, L);
    float NdotL = PBR_DOT(surface.N, L);

    vec3  numerator   = ndf * G * surface.F;
    float denominator = 4.0 * surface.NdotV * NdotL + PBR_EPSILON;
    vec3  specular    = numerator / denominator;

    float d = distance(light_position, fragpos);

    float attenuation = d / light.radius;
          attenuation = 1.0 - clamp(attenuation, 0.0, 1.0);

    vec3 result = ((surface.Kd * surface.albedo) / PBR_PI + specular) * light_diffuse * NdotL;

    return attenuation * strength * result;
}



vec3 IDK_PBR_Dirlight( IDK_Dirlight light, IDK_PBRSurfaceData surface, vec3 fragpos )
{
    const vec3 L = normalize(-light.direction.xyz);
    const vec3 H = normalize(surface.V + L);
    float NdotL  = max(dot(surface.N, L), 0.01);

    float ndf   = NDF(surface.roughness, surface.N, H);
    float G     = GSF(surface.roughness, surface.N, surface.V, L);

    vec3  numerator   = ndf * G * surface.F;
    float denominator = 4.0 * surface.NdotV * NdotL + PBR_EPSILON;
    vec3  specular    = numerator / denominator;

    vec3 radiance = light.diffuse.rgb;

    return ((surface.Kd * surface.albedo) / PBR_PI + specular) * radiance * NdotL * surface.ao;
}


vec3 IDK_PBR_Dirlight2( IDK_Dirlight light, IDK_PBRSurfaceData surface, vec3 fragpos )
{
    const vec3 L = normalize(-light.direction.xyz);
    const vec3 H = normalize(surface.V + L);
    float NdotL  = PBR_DOT(surface.N, L);

    float ndf   = NDF(surface.roughness, surface.N, H);
    float G     = GSF(surface.roughness, surface.N, surface.V, L);

    vec3  numerator   = ndf * G * surface.F;
    float denominator = 4.0 * surface.NdotV * NdotL + PBR_EPSILON;
    vec3  specular    = numerator / denominator;

    vec3 radiance = light.diffuse.rgb * light.diffuse.a;
         radiance = (radiance / PBR_PI) * fresnelDisney(surface.N, surface.V, L, surface.roughness);

    return ((surface.Kd * surface.albedo) / PBR_PI + specular) * radiance * NdotL;
}
