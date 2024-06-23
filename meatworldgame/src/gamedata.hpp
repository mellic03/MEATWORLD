#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>

#include <vector>

#include "systems/sys-player.hpp"

#include <meatnet.hpp>





namespace meatworld
{
    struct GameUI;
    struct GameSettings;
    struct GameData;
}


struct meatworld::GameUI
{
    idkui2::Grid *root;

    idkui2::List *mainmenu;
    idkui2::List *multiplayer;
    idkui2::List *settings;

    idkui2::Grid *inventory;
    idkui2::Grid *syndromes;

    std::string   texttest = "input";

    float slidertest =  0.0f;
    float slidermin  = -1.0f;
    float slidermax  = +1.0f;

};



struct meatworld::GameSettings
{
    int   win_width  = 1280;
    int   win_height = 720;
    float res_scale  = 1.0f;

};


struct meatworld::GameData
{
    meatworld::GameSettings settings;
    meatworld::GameUI       gameui;

    meatnet::Client        *meatnet = nullptr;
    std::string             meatnet_hostname = "127.0.0.1";
    meatnet::PlayerData     meatnet_playerdata;
    bool                    should_reset = false;

    int                     player;
    std::vector<int>        players;

    void init_multiplayer( const std::string &filepath );
    void reset_player();

};



