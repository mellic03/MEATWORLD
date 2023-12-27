#pragma once

#include "IDKengine/IDKengine.hpp"

#include <idk_imgui.hpp>

namespace EditorUI
{
    void dragVec3( std::string name, float *data, float min, float max,
                   float speed, const char *format, float default_value );

    void fileSelectPopup( const std::string &entry_path, std::string &selection, bool &open );

};

