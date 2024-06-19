
struct IDK_Camera
{
    vec4 position;
    mat4 V, P, PV, P_far, PV_far;
    vec4 image_size;
    vec4 image_plane;
    float exposure, gamma, shutter, bloom;

    vec2 chromatic_r;
    vec2 chromatic_g;
    vec2 chromatic_b;
    vec2 chromatic_pad;
    vec4 chromatic_strength;

    vec4 prev_position;
    mat4 prev_V, prev_P, prev_PV;
};


struct IDK_Dirlight
{
    mat4 transform;
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};


struct IDK_Pointlight
{
    mat4  transform;
    vec4  position;
    vec4  diffuse;
    vec3  attenuation;
    float radius;
};


struct IDK_Spotlight
{
    mat4  transform;
    vec4  position;
    vec4  orientation;
    vec4  direction;
    vec4  diffuse;
    vec4  attenuation;
    vec3  angle;
    float radius;
};


struct IDK_Atmosphere
{
    mat4    transform;
    vec4    position;
    vec4    sun_position;
    vec4    wavelengths;
    float   radius;
    float   density_falloff;
    float   scatter_strength;
    float   scale;
};



#define idk_renderdata_MAX_CAMERAS     4
#define idk_renderdata_MAX_DIRLIGHTS   1
#define idk_renderdata_MAX_POINTLIGHTS 16
#define idk_renderdata_MAX_SPOTLIGHTS  8
#define idk_renderdata_MAX_ATMOSPHERES 2

struct IDK_UBORenderData
{
    IDK_Camera      cameras     [idk_renderdata_MAX_CAMERAS];
    IDK_Dirlight    dirlights   [idk_renderdata_MAX_DIRLIGHTS];
    IDK_Pointlight  pointlights [idk_renderdata_MAX_POINTLIGHTS];
    IDK_Spotlight   spotlights  [idk_renderdata_MAX_SPOTLIGHTS];
    IDK_Atmosphere  atmospheres [idk_renderdata_MAX_ATMOSPHERES];
};


layout (std140, binding = 3) uniform IDK_UBO_RenderData
{
    IDK_UBORenderData un_RenderData;
};


IDK_Camera IDK_RenderData_GetCamera()
{
    return un_RenderData.cameras[0];
}


IDK_Dirlight IDK_RenderData_GetDirlight( int idx )
{
    return un_RenderData.dirlights[idx];
}


IDK_Pointlight IDK_RenderData_GetPointlight( int idx )
{
    return un_RenderData.pointlights[idx];
}


IDK_Spotlight IDK_RenderData_GetSpotlight( int idx )
{
    return un_RenderData.spotlights[idx];
}

