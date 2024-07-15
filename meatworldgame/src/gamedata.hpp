#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>

#include <vector>

#include "systems/sys-player.hpp"




namespace meatworld
{
    struct GameUI;
    struct GameSettings;
    struct GameData;
}


struct meatworld::GameUI
{
    idkui2::Grid *root;
    idkui2::Grid *mainmenu_root;
    idkui2::Grid *ingame_root;

    idkui2::List *mainmenu;
    idkui2::List *multiplayer;
    idkui2::List *settings;

    idkui2::Grid *inventory;
    idkui2::Grid *syndromes;

    float slidertest =  0.0f;
    float slidermin  = -1.0f;
    float slidermax  = +1.0f;

};



struct meatworld::GameSettings
{
    int   win_width  = 1280;
    int   win_height = 720;
    float res_scale  = 1.0f;
    float cam_fov    = 90.0f;

};


struct meatworld::GameData
{
    meatworld::GameSettings settings;
    meatworld::GameUI       gameui;

    std::string mp_hostname = "127.0.0.1";

    int                     player;
    std::vector<int>        players;
    std::vector<glm::vec4>  player_positions;

};



