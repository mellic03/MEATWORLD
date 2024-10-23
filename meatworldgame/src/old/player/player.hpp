#pragma once

#include "../character/character.hpp"
#include "../character/humanoid.hpp"


#include "player-controller.hpp"
#include "flashlight.hpp"
#include <meatnet.hpp>


namespace meatworld
{
    class Player;
    class EditorPlayer;
};





class meatworld::Player: public meatworld::Humanoid
{
private:

public:
    // int m_cam_obj    = -1;
    int m_hinge_obj  = -1;
    int m_weapon_obj = -1;

    Player( const glm::vec3 &position, float yaw );
    ~Player();

    virtual void move( idk::EngineAPI &api, const CharacterMotion &motion );


};


// class meatworld::Player: public meatworld::CharacterBase
// {
// private:
//     int m_audio_emitter = -1;

// public:
//     int m_torso_obj = -1;
//     int m_head_obj  = -1;

//     int m_grabbed_obj = -1;
//     int m_hinge_obj   = -1;
//     int m_hit_obj     = -1;
//     int m_cam_obj     = -1;

//     float m_health = 10.0f;

//     float m_run_speed  = 3.5f;
//     float m_walk_speed = 2.5f;
//     float m_jump_force = 1.0f;

//     Flashlight m_flashlight;
//     RangedWeapon *m_weapon = nullptr;



//     Player();
//     ~Player();

//     virtual void move( idk::EngineAPI&, const CharacterMotion& );
//     void update( idk::EngineAPI& );
//     void update( idk::EngineAPI&, meatnet::PeerData& );

//     template <typename T>
//     void giveWeapon()
//     {
//         if (m_weapon)
//         {
//             delete m_weapon;
//         }

//         m_weapon = new T(m_cam_obj, m_hit_obj);
//     }

// };




// class meatworld::EditorPlayer: public meatworld::Player
// {
// public:

//     void update( idk::EngineAPI& );
//     virtual void move( idk::EngineAPI&, const CharacterMotion& ) final {  };
// };

