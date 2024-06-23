#include "gamedata.hpp"




void
meatworld::GameData::init_multiplayer( const std::string &filepath )
{
    using namespace idk;

    std::cout << "[MEATWORLD] Initializing multiplayer" << std::endl;
    ECS2::load(filepath);


    player = -1;
    // player = ECS2::createGameObject("Player", false);
    // std::cout << "Created player with object ID " << player << "\n";
    // ECS2::giveComponent<PlayerControllerCmp>(player);


    // players.resize(4);

    // for (int i=0; i<4; i++)
    // {
    //     players[i] = ECS2::createGameObject("Player " + std::to_string(i), false);
    //     ECS2::giveComponent<OLPlayerControllerCmp>(players[i]);
    // }
}



void
meatworld::GameData::reset_player()
{
    using namespace idk;

    player = ECS2::createGameObject("Player", false);
    std::cout << "Created player with object ID " << player << "\n";
    ECS2::giveComponent<PlayerControllerCmp>(player);

    players.resize(4);

    for (int i=0; i<4; i++)
    {
        players[i] = ECS2::createGameObject("Player " + std::to_string(i), false);
        ECS2::giveComponent<OLPlayerControllerCmp>(players[i]);
    }
}
