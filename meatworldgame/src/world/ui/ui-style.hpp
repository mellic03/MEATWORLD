#pragma once

#include <IDKGraphics/UI/idk_ui.hpp>

namespace meat
{
    class CharacterDrone;
}


namespace meatui
{
    static const glm::vec4 color_primary   = glm::vec4( 0.25f, 0.25f, 0.25f, 0.95f );
    static const glm::vec4 color_secondary = glm::vec4( 0.5f,  0.5f,  0.5f,  0.95f );
    static const glm::vec4 color_tertiary  = glm::vec4( 0.8f,  0.8f,  0.8f,  0.95f );
    static const glm::vec4 color_blue      = glm::vec4(0.63, 0.8, 1.0, 1.0);

    static const glm::vec4 color_text      = glm::vec4( 1.0f );

    static const glm::vec4 color_bg0  = glm::vec4( 0.15,  0.15,  0.25, 0.825 );
    static const glm::vec4 color_bg1  = glm::vec4( 0.2,   0.2,   0.3,  0.825 );
    static const glm::vec4 color_bg2  = glm::vec4( 0.25,  0.25,  0.35, 0.825 );

    static const glm::vec4 color_fg0  = glm::vec4( 0.45,  0.45,  0.55, 0.825 );
    static const glm::vec4 color_fg1  = glm::vec4( 0.5,   0.5,   0.6,  0.825 );
    static const glm::vec4 color_fg2  = glm::vec4( 0.55,  0.55,  0.65, 0.825 );
    static const glm::vec4 color_fg3  = glm::vec4( 0.6,   0.6,   0.7,  0.825 );



    static const
    idkui::Style root_style = {
        .margin  = glm::vec4(0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = glm::vec4(0.0f),
        .fg      = glm::vec4(0.0f),
        .bg      = glm::vec4(0.0f)
    };

    static const
    idkui::Style window_style = {
        .margin  = glm::vec4(0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = glm::vec4(0.0f),
        .fg      = color_bg2,
        .bg      = color_fg3
    };

    static const
    idkui::Style grid_style = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(16.0f),
        .radius  = glm::vec4(4.0f),
        .fg      = color_fg1,
        .bg      = color_bg1
    };

    static const
    idkui::Style crosshair_style = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(16.0f),
        .radius  = glm::vec4(4.0f),
        .fg      = color_fg1,
        .bg      = color_bg1
    };

    static const
    idkui::Style gridsquare_style = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(4.0f),
        .radius  = glm::vec4(16.0f),
        .fg      = color_blue,
        .bg      = color_bg1
    };



    static const
    idkui::Style menu_style = {
        .margin  = glm::vec4(16.0f),
        .padding = glm::vec4(16.0f),
        .radius  = glm::vec4(4.0f),
        .fg      = color_bg2,
        .bg      = color_bg2,
        .maxbounds = glm::vec2(900.0f, +INFINITY)
    };


    static const
    idkui::Style menutitle_style = {
        .margin  = glm::vec4(0.0f),
        .padding = glm::vec4(64.0f),
        .radius  = glm::vec4(0.0f),
        .fg      = color_bg2,
        .bg      = color_bg2,
        .minbounds = glm::vec2(0.0f, 64.0f),
        .maxbounds = glm::vec2(+INFINITY, 64.0f)
    };


    static const
    idkui::Style button_style = {
        .margin  = glm::vec4(2.0f),
        .padding = glm::vec4(2.0f),
        .radius  = glm::vec4(8.0f),
        .fg      = color_fg3,
        .bg      = color_bg1,
        .minbounds = glm::vec2(0.0f, 64.0f),
        .maxbounds = glm::vec2(+INFINITY, 64.0f)
    };


    static const
    idkui::Style buttontext_style = {
        .margin  = glm::vec4(2.0f),
        .padding = glm::vec4(0.0f),
        .radius  = glm::vec4(0.0f),
        .fg      = color_bg1,
        .bg      = color_fg3
    };


    static const
    idkui::Style onscreentext_style = {
        .margin  = glm::vec4(0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = glm::vec4(0.0f),
        .fg      = glm::vec4(1.0f)
    };


    // static const
    // idkui::ElementStyle root_style = {
    //     .invisible    = true,
    //     .default_open = true,
    //     .radius       = 0.0f
    // };


    // static const
    // idkui::ElementStyle mainmenu_style = {
    //     .invisible = true,
    //     .margin  = glm::vec4(8.0f, 8.0f, 0.0f, 0.0f),
    //     .padding = glm::vec4(0.0f),
    //     .radius  = 8.0f,
    //     .fg      = glm::vec4(1.0f),
    //     .bg      = color_primary
    // };


    // static const
    // idkui::ElementStyle menu_style = {
    //     .margin  = glm::vec4(8.0f, 8.0f, 0.0f, 0.0f),
    //     .padding = glm::vec4(0.0f),
    //     .radius  = 8.0f,
    //     .fg      = color_tertiary,
    //     .bg      = color_primary
    // };


    // static const
    // idkui::ElementStyle submenu_style = {
    //     .margin  = glm::vec4(8.0f, 8.0f, 0.0f, 0.0f),
    //     .padding = glm::vec4(0.0f),
    //     .radius  = 8.0f,
    //     .fg      = glm::vec4(1.0f),
    //     .bg      = color_primary
    // };


    // static const
    // idkui::ElementStyle splitwindow_style = {
    //     .margin    = glm::vec4(0.0f),
    //     .radius    = 0.0f,
    //     .fg        = glm::vec4(1.0f),
    //     .bg        = color_primary
    // };


    // static const
    // idkui::ElementStyle title_style = {
    //     .margin = glm::vec4(4.0f),
    //     .radius = 16.0f,
    //     .fg     = glm::vec4(1.0f),
    //     .bg     = glm::vec4(0.0f)
    // };

    // static const
    // idkui::ElementStyle label_style = {
    //     .margin = glm::vec4(4.0f),
    //     .radius = 0.0f,
    //     .fg     = glm::vec4(1.0f),
    //     .bg     = color_primary
    // };

    // static const
    // idkui::ElementStyle onScreenText_style = {
    //     .margin  = glm::vec4(1.0f),
    //     .padding = glm::vec4(0.0f),
    //     .radius  = 0.0f,
    //     .fg      = glm::vec4(1.0, 1.0, 1.0, 0.0),
    //     .bg      = glm::vec4(0.0, 0.0, 0.0, 0.0)
    // };

    // static const
    // idkui::ElementStyle list_style = {
    //     .margin  = glm::vec4(16.0f, 16.0f, 0.0f, 0.0f),
    //     .padding = glm::vec4(0.0f),
    //     .radius  = 8.0f,
    //     .fg      = glm::vec4(1.0f),
    //     .bg      = color_primary
    // };


    // static const
    // idkui::ElementStyle button_style = {
    //     .margin = glm::vec4(4.0f),
    //     .radius = 8.0f,
    //     .fg     = color_tertiary,
    //     .bg     = color_secondary
    // };


    // static const
    // idkui::ElementStyle slider_style = {
    //     .margin = glm::vec4(4.0f),
    //     .radius = 8.0f,
    //     .fg     = color_fg1,
    //     .bg     = color_bg1,
    //     .fg2    = color_text,
    //     .bg2    = color_bg2
    // };


    // static const
    // idkui::ElementStyle textinput_style = {
    //     .margin = glm::vec4(4.0f),
    //     .radius = 8.0f,
    //     .fg     = glm::vec4(1.0f),
    //     .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.5f),
    //     .fg2    = glm::vec4(0.8f, 0.8f, 0.8f, 0.5f),
    //     .bg2    = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f)
    // };
}

