#ifndef IDK_VOLUMETRIC
#define IDK_VOLUMETRIC



struct IDK_SphereVolume
{
    int   samples_view;
    int   samples_sun;
    float density;
    float scatter_coeff;
    float absorb_coeff;

    float noise_amp;
    float noise_wav;
};



float IDK_PHG( float g, float cosTheta )
{
    const float Inv4Pi = 0.07957747154594766788;
    
    float gSq = g * g;
    float denomPreMul = 1 + gSq - (2.0 * g * cosTheta);
    return (1 - gSq) * Inv4Pi * inversesqrt(denomPreMul * denomPreMul * denomPreMul);
}


float IDK_MieScattering( float cosTheta )
{
    return mix(IDK_PHG(0.8, cosTheta), IDK_PHG(-0.5, cosTheta), 0.5);
}


float IDK_RayleighScattering( float cosTheta )
{
    return ((3.0*3.14159) / 16.0) * (1.0 + cosTheta*cosTheta);
}


vec3 SkyColor( float sunAmount )
{
    // Simple gradient between blue sky and sunset colors
    vec3 skyDayColor = mix(vec3(0.5, 0.7, 1.0), vec3(0.2, 0.5, 1.0), pow(sunAmount, 2.0)); // Horizon to overhead gradient
    vec3 skySunsetColor = mix(vec3(1.0, 0.5, 0.2), vec3(0.1, 0.2, 0.5), pow(sunAmount, 2.0)); // Sunset gradient

    return mix(skySunsetColor, skyDayColor, pow(sunAmount, 4.0)); // Blend day and sunset colors
}

vec3 SunColor( float sunAmount )
{
    // Bright white at noon, orange to red at sunrise/sunset
    vec3 middaySun = vec3(1.0, 1.0, 0.9); // Slight yellowish-white
    vec3 sunsetSun = vec3(1.0, 0.5, 0.3); // Orange-red

    return mix(sunsetSun, middaySun, pow(sunAmount, 8.0)); // Blend sun colors based on angle
}




float VolumetricFogAlpha( float a, float b, vec3 ro, vec3 rd, float frag_dist )
{
    return (a/b) * exp(-ro.y * b) * (1.0 - exp(-frag_dist * rd.y * b)) / rd.y;
}



vec3 VolumetricFogColor( vec3 L, vec3 rd )
{
    float sunAmount = max( dot(rd, L), 0.0 );
    vec3  fogColor  = mix( vec3(0.5,0.6,0.7), // blue
                           vec3(1.0,0.9,0.7), // yellow
                           pow(sunAmount, 8.0) );
    return fogColor;
}



// vec3 VolumetricFogColor( IDK_Dirlight light, vec3 rd )
// {
//     float sunAmount = max(dot(rd, -light.direction.xyz), 0.0);
//     float upness = max(dot(rd, vec3(0, 1, 0)), 0.0);

//     float rayleigh = IDK_RayleighScattering(max(dot(rd, -light.direction.xyz), 0.0));

//     vec3 sky = pow(vec3(0.5, 0.6, 0.7), vec3(2.5));

//     // sky = mix(sky, sky*0.4, (1.0-rayleigh)*sunAmount);

//     vec3 sun = pow(vec3(1.0, 0.9, 0.7), vec3(1.0));

//     vec3  color     = mix( sky, // blue
//                            sun, // yellow
//                            pow(sunAmount, 256.0) );

//     return color*rayleigh;
// }


#endif
