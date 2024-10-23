#pragma once

#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKECS/IDKECS.hpp>

#include "../../../meatnet/src/meatnet.hpp"
#include "../gamedata.hpp"


void createInGame      ( idk::EngineAPI&, idkui::LayoutManager*, meatworld::GameData* );
void createMainMenu    ( idk::EngineAPI&, idkui::LayoutManager*, meatworld::GameData* );
void createMenu        ( idk::EngineAPI&, idkui::LayoutManager*, meatworld::GameData* );
void createMultiplayer ( idk::EngineAPI&, idkui::LayoutManager*, meatworld::GameData* );
void createSettings    ( idk::EngineAPI&, idkui::LayoutManager*, meatworld::GameData* );
void createSyndromes   ( idk::EngineAPI&, idkui::LayoutManager*, meatworld::GameData* );



namespace meatui
{
    static const glm::vec4 color_primary   = glm::vec4( 0.25f, 0.25f, 0.25f, 0.95f );
    static const glm::vec4 color_secondary = glm::vec4( 0.5f,  0.5f,  0.5f,  0.95f );
    static const glm::vec4 color_tertiary  = glm::vec4( 0.8f,  0.8f,  0.8f,  0.95f );
    static const glm::vec4 color_text      = glm::vec4( 1.0f );

    static const glm::vec4 color_bg0  = glm::vec4( 0.15,  0.15,  0.15,  0.95 );
    static const glm::vec4 color_bg1  = glm::vec4( 0.2,   0.2,   0.2,   0.95 );
    static const glm::vec4 color_bg2  = glm::vec4( 0.25,  0.25,  0.25,  0.95 );


    static const glm::vec4 color_fg0  = glm::vec4( 0.45,  0.45,  0.45,  0.95 );
    static const glm::vec4 color_fg1  = glm::vec4( 0.5,   0.5,   0.5,   0.95 );
    static const glm::vec4 color_fg2  = glm::vec4( 0.55,  0.55,  0.55,  0.95 );


    static const
    idkui::ElementStyle root_style = {
        .invisible    = true,
        .default_open = false,
        .radius       = 0.0f
    };


    static const
    idkui::ElementStyle mainmenu_style = {
        .invisible = true,
        .margin  = glm::vec4(8.0f, 8.0f, 0.0f, 0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = color_primary
    };


    static const
    idkui::ElementStyle submenu_style = {
        .margin  = glm::vec4(8.0f, 8.0f, 0.0f, 0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = color_primary
    };


    static const
    idkui::ElementStyle splitwindow_style = {
        .margin    = glm::vec4(0.0f),
        .radius    = 0.0f,
        .fg        = glm::vec4(1.0f),
        .bg        = color_primary
    };


    static const
    idkui::ElementStyle title_style = {
        .margin = glm::vec4(4.0f),
        .radius = 16.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.0f)
    };

    static const
    idkui::ElementStyle label_style = {
        .margin = glm::vec4(4.0f),
        .radius = 0.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = color_primary
    };


    static const
    idkui::ElementStyle list_style = {
        .margin  = glm::vec4(16.0f, 16.0f, 0.0f, 0.0f),
        .padding = glm::vec4(0.0f),
        .radius  = 8.0f,
        .fg      = glm::vec4(1.0f),
        .bg      = color_primary
    };


    static const
    idkui::ElementStyle button_style = {
        .margin = glm::vec4(4.0f),
        .radius = 8.0f,
        .fg     = color_tertiary,
        .bg     = color_secondary
    };


    static const
    idkui::ElementStyle slider_style = {
        .margin = glm::vec4(4.0f),
        .radius = 8.0f,
        .fg     = color_fg1,
        .bg     = color_bg1,
        .fg2    = color_text,
        .bg2    = color_bg2
    };


    static const
    idkui::ElementStyle textinput_style = {
        .margin = glm::vec4(4.0f),
        .radius = 8.0f,
        .fg     = glm::vec4(1.0f),
        .bg     = glm::vec4(0.25f, 0.25f, 0.25f, 0.5f),
        .fg2    = glm::vec4(0.8f, 0.8f, 0.8f, 0.5f),
        .bg2    = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f)
    };
}

