#pragma once

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>


namespace idkg
{
    enum GameStateIndex: uint32_t
    {
        GameState_PROGRESSION    = 0,
        GameState_CURRENT_PLANET = 1,
        GameState_CURRENT_REGION = 2
    };

    enum GameStatePlanet: uint32_t
    {
        PLANET_SUN    = 1,
        PLANET_MOON   = 2,
        PLANET_DOWNUS
    };

    enum GameStateRegion: uint32_t
    {
        MOON_REGION_PLAYER_HOME    = 1,
        MOON_REGION_MEDICAL_OFFICE = 2,

        DOWNUS_REGION_GOVERNMENT_OFFICE = 2
    };


    void update_blambo( idk::EngineAPI &api );
    void update_player( idk::EngineAPI &api );


    void pew( idk::EngineAPI &api );

};

