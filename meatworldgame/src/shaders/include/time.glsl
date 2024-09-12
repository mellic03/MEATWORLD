#include "./bindings.glsl"

#ifndef IDK_TIME
#define IDK_TIME


layout (std140, binding = IDK_BINDING_UBO_Time) uniform IDK_UBO_Time
{
    vec4 IDK_UBO_time;
};


float IDK_GetTime()
{
    return IDK_UBO_time[0];
}


float IDK_GetDeltaTime()
{
    return IDK_UBO_time[1];
}


#endif
