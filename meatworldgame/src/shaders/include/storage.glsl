#extension GL_ARB_bindless_texture: require

#define MAX_CAMERAS     1
#define MAX_DIRLIGHTS   1
#define MAX_POINTLIGHTS 32
#define MAX_SPOTLIGHTS  32

#define MAX_TEXTURES   (4*4096)
#define MAX_TRANSFORMS 1024
#define MAX_DRAW_CALLS 4096


struct IDK_Camera
{
    vec4 position;
    mat4 P, V;
    mat4 prev_P, prev_V;

    float width, height, near, far;
    float exposure, gamma, shutter, pad0;
    float fov, aspect, bloom, pad1;
};


struct IDK_Dirlight
{
    mat4 transform;
    mat4 transforms[4];
    vec4 cascades;

    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};


struct IDK_Pointlight
{
    vec4  position;
    vec4  diffuse;
    vec3  attenuation;
    float radius;
};


struct IDK_Spotlight
{
    vec4  position;
    vec4  direction;
    vec4  diffuse;
    vec4  attenuation;
    vec3  angle;
    float radius;
};


layout (std140, binding = 0) uniform IDK_UBO_Lightsources
{
    IDK_Camera      IDK_UBO_cameras     [MAX_CAMERAS];
    IDK_Dirlight    IDK_UBO_dirlights   [MAX_DIRLIGHTS];
    IDK_Pointlight  IDK_UBO_pointlights [MAX_POINTLIGHTS];
    IDK_Spotlight   IDK_UBO_spotlights  [MAX_SPOTLIGHTS];
    int             IDK_UBO_counter;
};


layout (std430, binding = 0) readonly buffer IDK_SSBO_Indirect
{
    sampler2D   IDK_SSBO_textures          [MAX_TEXTURES];
    mat4        IDK_SSBO_transforms        [MAX_TRANSFORMS];
    uint        IDK_SSBO_transform_offsets [MAX_DRAW_CALLS];
    uint        IDK_SSBO_texture_offsets   [MAX_DRAW_CALLS];
};



IDK_Camera IDK_RenderData_GetCamera()
{
    return IDK_UBO_cameras[0];
}


IDK_Dirlight IDK_RenderData_GetDirlight( int idx )
{
    return IDK_UBO_dirlights[idx];
}


IDK_Pointlight IDK_RenderData_GetPointlight( int idx )
{
    return IDK_UBO_pointlights[idx];
}


IDK_Spotlight IDK_RenderData_GetSpotlight( int idx )
{
    return IDK_UBO_spotlights[idx];
}

