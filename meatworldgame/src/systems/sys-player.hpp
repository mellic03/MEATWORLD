#pragma once

#include <IDKECS/IDKECS.hpp>
#include "../../../meatnet/src/meatnet.hpp"

#include "../player/weapon.hpp"
#include "../player/flashlight.hpp"


namespace idk
{
    struct PlayerSpawnCmp;
}


namespace meatworld
{
    struct NPCConfig;
    struct NPCCmp;
    class  PlayerSys;
}


struct meatworld::NPCConfig
{
    std::string model_path = "assets/models/dr-SDS.idkvi";
};



class meatworld::PlayerSys: public idk::ECS2::System
{
private:
    inline static meatnet::Client *m_client = nullptr;


public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static void init_multiplayer( const std::string &host );
    static void update_player();

};


struct idk::PlayerSpawnCmp
{
    int obj_id = -1;

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};


struct meatworld::NPCCmp
{
    int obj_id = -1;

    NPCConfig config;

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};





