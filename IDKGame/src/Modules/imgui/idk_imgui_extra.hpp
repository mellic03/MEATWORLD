#pragma once

#include "IDKengine/IDKengine.hpp"
#include "imgui-1.89.7/imgui.h"
#include "imgui-1.89.7/imgui_impl_sdl2.h"
#include "imgui-1.89.7/imgui_impl_opengl3.h"
#include "imgui-1.89.7/imgui_stdlib.h"


namespace idkgui
{
    void dragVec3( std::string name, float *data, float min, float max,
                   float speed, const char *format, float default_value );

};

