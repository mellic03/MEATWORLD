#pragma once

#include <IDKengine/IDKengine.hpp>
#include <idk_imgui.hpp>

#include "common/idk_imgui_extra.hpp"
#include "icon-headers/idk_Icons.hpp"


class EditorUI_Module: public idk::Module
{
private:
    static constexpr int NUM_FONTS = 4;

    const std::array<int, NUM_FONTS> FONT_SIZES = { 14, 16, 18, 20 };
    std::array<ImFont *, NUM_FONTS> m_fonts;

    ImFont *    m_font;
    ImFont *    m_icons;

    bool        m_show_ImGui_demo = false;

    // Editor Tabs
    // -----------------------------------------------------------------------------------------
    int         m_selected_object = -1;

    void        _tab_gameobjects( idk::Engine & );
    void        _tab_inspect( idk::Engine &, int object_id );
    void        _tab( idk::Engine & );
    // -----------------------------------------------------------------------------------------

    // Menu Bar
    // -----------------------------------------------------------------------------------------
    void        _menubar( idk::Engine & );
    void        _menubar_settings( idk::Engine & );
    // -----------------------------------------------------------------------------------------


public:

    void        init( idk::Engine & );
    void        stage_A( idk::Engine & ) {  };
    void        stage_B( idk::Engine & );
    void        stage_C( idk::Engine & ) {  };

};

