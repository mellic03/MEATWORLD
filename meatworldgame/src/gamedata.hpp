#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>

#include <vector>

#include "systems/sys-player.hpp"




namespace meatworld
{
    struct GameUI;
    struct GameData;
}

struct meatworld::GameUI
{
    idkui2::Grid *root;

    idkui2::Grid *mainmenu;
    idkui2::Grid *multiplayer;
    idkui2::Grid *settings;

    idkui2::Grid *inventory;
    idkui2::Grid *syndromes;

    std::string   texttest = "input";

    float slidertest =  0.0f;
    float slidermin  = -1.0f;
    float slidermax  = +1.0f;

};


struct meatworld::GameData
{

    meatworld::GameUI gameui;

    int               player;
    std::vector<int>  players;

    void init_multiplayer( const std::string &filepath )
    {
        using namespace idk;

        std::cout << "[MEATWORLD] Initializing multiplayer" << std::endl;
        ECS2::load(filepath);


        player = ECS2::createGameObject("Player", false);
        ECS2::giveComponent<PlayerControllerCmp>(player);


        players.resize(4);

        for (int i=0; i<4; i++)
        {
            players[i] = ECS2::createGameObject("Player " + std::to_string(i), false);
            ECS2::giveComponent<OLPlayerControllerCmp>(players[i]);
        }
    }

};



