#include "sys-player.hpp"

#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>

#include <libidk/idk_log.hpp>

#include "sys-weapon.hpp"
#include "../player/ol_player.hpp"
#include "../game.hpp"



// static meatworld::OLPlayer OL_Players[4];


void
meatworld::PlayerSys::init( idk::EngineAPI &api )
{
    using namespace idk;

}


void
meatworld::PlayerSys::update( idk::EngineAPI &api )
{
    using namespace idk;

    // if (m_client == nullptr || MeatWorldGame::player == nullptr)
    // {
    //     return;
    // }


    // static meatnet::PeerData player_data;
    // static meatnet::PeerData peer_data[4];

    // MeatWorldGame::player->update(api, player_data);
    // m_client->send(player_data);

    // if (m_client->recv(&peer_data[0]))
    // {
    //     for (int i=0; i<4; i++)
    //     {
    //         OL_Players[i].update(api, peer_data[i]);

    //         if (OL_Players[i].m_health < 0.0f)
    //         {
    //             OL_Players[i].init();
    //         }
    //     }
    // }
}



void
meatworld::PlayerSys::init_multiplayer( const std::string &host )
{
    using namespace idk;

    // m_client = new meatnet::Client(
    //     "Michael", "127.0.0.1", 4201, [](std::string){}
    // );

    // for (int i=0; i<4; i++)
    // {
    //     OL_Players[i].init();
    // }
}







size_t
idk::PlayerSpawnCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
idk::PlayerSpawnCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
idk::PlayerSpawnCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::PlayerSpawnCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
idk::PlayerSpawnCmp::onObjectCopy( int src_obj, int dst_obj )
{

}






size_t
meatworld::NPCCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
meatworld::NPCCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
meatworld::NPCCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    using namespace idk;

    auto &cmp = ECS2::getComponent<NPCCmp>(obj_id);
    ECS2::giveComponent<ModelCmp>(obj_id);
    ECS2::giveComponent<KinematicCapsuleCmp>(obj_id);

    ModelSys::assignModel(obj_id, cmp.config.model_path);
}


void
meatworld::NPCCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::NPCCmp::onObjectCopy( int src_obj, int dst_obj )
{

}



















// size_t
// meatworld::OLPlayerCmp::serialize( std::ofstream &stream ) const
// {
//     size_t n = 0;
//     n += idk::streamwrite(stream, obj_id);
//     return n;
// }


// size_t
// meatworld::OLPlayerCmp::deserialize( std::ifstream &stream )
// {
//     size_t n = 0;
//     n += idk::streamread(stream, obj_id);
//     return n;
// }


// void
// meatworld::OLPlayerCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
// {
//     using namespace idk;

//     auto &cmp = ECS2::getComponent<OLPlayerCmp>(obj_id);
//     ECS2::giveComponent<ModelCmp>(obj_id);
//     ModelSys::assignModel(obj_id, "assets/models/head.idkvi");
//     TransformSys::getXYZScale(obj_id) = glm::vec3(0.6f);


//     cmp.light = ECS2::createGameObject("light", false);
//     ECS2::giveComponent<SpotlightCmp>(cmp.light);
//     ECS2::getComponent<SpotlightCmp>(cmp.light).light.diffuse = glm::vec4(0.0f);
//     ECS2::getComponent<SpotlightCmp>(cmp.light).light.radius  = 5.0f;
//     ECS2::getComponent<SpotlightCmp>(cmp.light).light.angle   = glm::vec3(0.7f);

//     cmp.weapon_obj = ECS2::createGameObject("weapon", false);

//     ECS2::giveChild(obj_id, cmp.weapon_obj);
//     ECS2::giveChild(cmp.weapon_obj, cmp.light);

//     ECS2::giveComponent<ModelCmp>(cmp.weapon_obj);
//     ECS2::giveComponent<WeaponCmp>(cmp.weapon_obj);
//     ModelSys::assignModel(cmp.weapon_obj, "assets/models/weapons/hl2-ar2.idkvi");
//     WeaponSys::config(cmp.weapon_obj, {0.05f, 6});

//     auto &tcmp = TransformSys::getTransformCmp(cmp.weapon_obj);
//     tcmp.yaw = M_PI;
//     tcmp.transform.position = glm::vec3(-0.15f, -0.14f, 0.05f);
//     tcmp.transform.scale    = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
// }


// void
// meatworld::OLPlayerCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
// {

// }


// void
// meatworld::OLPlayerCmp::onObjectCopy( int src_obj, int dst_obj )
// {

// }


