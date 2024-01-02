#pragma once

#define IDK_IMGUI_DOCKING

#ifdef IDK_IMGUI_DOCKING
    #include "../imgui-docking/imgui.h"
    #include "../imgui-docking/imgui_impl_sdl2.h"
    #include "../imgui-docking/imgui_impl_opengl3.h"
    #include "../imgui-docking/imgui_stdlib.h"
#else
    #include "../imgui-1.90/imgui.h"
    #include "../imgui-1.90/imgui_impl_sdl2.h"
    #include "../imgui-1.90/imgui_impl_opengl3.h"
    #include "../imgui-1.90/imgui_stdlib.h"
#endif


