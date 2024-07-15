#pragma once

#include <libidk/idk_assert.hpp>
#include <libidk/idk_module.hpp>
#include <libidk/idk_game.hpp>

#include <string>

#include "gamedata.hpp"
#include "player/player.hpp"



class MeatWorldGame: public idk::Game
{
public:

    meatworld::GameData gamedata;
    inline static meatworld::Player *player = nullptr;

    MeatWorldGame(): idk::Game("MEATWORLD") {  };

    virtual void registerModules ( idk::EngineAPI& ) final;
    virtual void setup           ( const std::vector<std::string>&, idk::EngineAPI& ) final;
    virtual void mainloop        ( idk::EngineAPI& ) final;
    virtual void shutdown        (                 ) final;


    void registerPrefabs();
    void onSceneLoad();

};

