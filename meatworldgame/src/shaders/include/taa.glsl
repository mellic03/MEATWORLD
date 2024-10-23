
#ifndef IDK_TAA
#define IDK_TAA

#include "../include/time.glsl"
#include "../include/noise.glsl"


// vec2 haltonSequence[6];
vec2 haltonSequence[16] = vec2[16]
(
    vec2(0.500000, 0.333333),
    vec2(0.250000, 0.666667),
    vec2(0.750000, 0.111111),
    vec2(0.125000, 0.444444),
    vec2(0.625000, 0.777778),
    vec2(0.375000, 0.222222),
    vec2(0.875000, 0.555556),
    vec2(0.062500, 0.888889),
    vec2(0.562500, 0.037037),
    vec2(0.312500, 0.370370),
    vec2(0.812500, 0.703704),
    vec2(0.187500, 0.148148),
    vec2(0.687500, 0.481481),
    vec2(0.437500, 0.814815),
    vec2(0.937500, 0.259259),
    vec2(0.031250, 0.592593)
);



// float _createHaltonSequence( int index, int base )
// {
// 	float f = 1;
// 	float r = 0;
// 	int current = index;
// 	do 
// 	{
// 		f = f / base;
// 		r = r + f * (current % base);
// 		current = int(floor(current / base));
// 	} while (current > 0);

//       return r;
// }


// void CreateHaltonSequence()
// {
//     // for (int i=0; i<32; i++)
//     // {
//     //     haltonSequence[i].x = IDK_BlueNoise(vec2(i, 0.1)/2048.0).r;
//     //     haltonSequence[i].y = IDK_BlueNoise(vec2(0.1, i)/2048.0).r;
//     // }

//     for (int iter = 0; iter < 6; iter++)
//     {
//         haltonSequence[iter] = vec2(
//             _createHaltonSequence(iter + 1, 2),
//             _createHaltonSequence(iter + 1, 3)
//         );
//     }
// }

// vec2 IDK_Jitter( float width, float height )
// {
//     // CreateHaltonSequence();

// 	uint idx = IDK_GetFrame() % 16;
//     vec2 size   = vec2(width, height);
//     vec2 jitter = haltonSequence[idx] * 2.0 - 1.0;
//          jitter.x = jitter.x / width;
//          jitter.y = jitter.y / height;

//     return 1.0 * jitter;
// }


// mat4 IDK_JitterMat( float width, float height )
// {
//     vec2 jitter    = IDK_Jitter(width, height);
//     mat4 jittermat = mat4(1.0);
// 	// vec2 jitter    = haltonSequence[idx] / vec2(width, height);

//     {
//         jittermat[3][0] += jitter.x;
//         jittermat[3][1] += jitter.y;
//     }

//     return jittermat;
// }


struct IDK_VelocityData
{
    vec4 curr,  prev;
    vec4 wcurr, wprev;
};


IDK_VelocityData PackVData( IDK_Camera cam, vec3 curr_wspace, vec3 prev_wspace )
{
    IDK_VelocityData data;

    data.curr  = cam.P_nojitter * cam.V * vec4(curr_wspace, 1.0);
    data.prev  = cam.prev_P_nojitter * cam.prev_V * vec4(prev_wspace, 1.0);
    data.wcurr = cam.P_nojitter * cam.V * vec4(curr_wspace, 1.0);
    data.wprev = cam.P_nojitter * cam.V * vec4(prev_wspace, 1.0);

    return data;
}


vec4 PackVelocity( IDK_VelocityData vdata )
{
    vec2 curr  = (vdata.curr.xy / vdata.curr.w) * 0.5 + 0.5;
    vec2 prev  = (vdata.prev.xy / vdata.prev.w) * 0.5 + 0.5;

    vec2 wcurr = (vdata.wcurr.xy / vdata.wcurr.w) * 0.5 + 0.5;
    vec2 wprev = (vdata.wprev.xy / vdata.wprev.w) * 0.5 + 0.5;

    return 10.0 * vec4(curr-prev, wcurr-wprev);

    // vec4 v = vec4(curr-prev, wcurr-wprev) * 0.5 + 0.5;
    // return pow(v, vec4(3.0));
}

vec4 UnpackVelocity( vec4 vel )
{
    return (1.0 / 10.0) * vel;

    // vec4 v = pow(vel, vec4(1.0/3.0));
    // return v * 2.0 - 1.0;
}


#endif
