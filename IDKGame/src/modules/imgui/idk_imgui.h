#pragma once

#include "IDKengine.h"
#include "imgui-1.89.7/imgui.h"
#include "imgui-1.89.7/imgui_impl_sdl2.h"
#include "imgui-1.89.7/imgui_impl_opengl3.h"


class ImGui_Module: public idk::Module
{
private:
    
    std::string m_menu_action = "";

    void        f_settings_graphics();
    void        f_main_menu_bar();

public:
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & );
    void        stage_B( idk::Engine & );

};

