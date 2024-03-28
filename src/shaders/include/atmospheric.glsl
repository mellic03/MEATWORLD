
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



struct IDK_AtmosphericProperties
{
    float Hr;
    float Hm;

    vec3 Br;
    vec3 Bm;
};


float IDK_AtmosphericDensity( float altitude, float max_altitude )
{
    float base = 4.0;

    float d = 1.0 - clamp(altitude / max_altitude, 0.0, 1.0);
    return d * base * exp(-altitude / max_altitude);
}


vec3 IDK_AtmosphericMie( float cosTheta )
{
    vec3 base = vec3(210e-1);
    return base * IDK_MieScattering(cosTheta);
}



vec3 IDK_AtmosphericRayleigh( float cosTheta )
{
    vec3 base = vec3(33.1e-2, 13.5e-2, 5.8e-2);
    return base * IDK_RayleighScattering(cosTheta);
}

