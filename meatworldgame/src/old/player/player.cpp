#include "player.hpp"

#include <IDKAudio/IDKAudio.hpp>
#include <IDKBuiltinCS/sys-audio.hpp>

#include <libidk/idk_math.hpp>

#include "../systems/sys-grabbable.hpp"
#include "../systems/sys-weapon.hpp"

#include "../game.hpp"
#include "../../../meatnet/src/meatnet.hpp"
#include "../systems/sys-player.hpp"


using namespace idk;



meatworld::CharacterMotion
meatworld::PlayerController::getMovement( idk::EngineAPI &api, CharacterBase* )
{
    float dtime = api.dtime();
    auto &ren   = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    CharacterMotion motion;

    if (events.mouseCaptured() == false)
    {
        return motion;
    }

    static const glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
    static const glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    static const glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 delta  = glm::vec3(0.0f);
    glm::vec2 dmouse = -0.1f * events.mouseDelta();

    if (K.keyDown(idk::Keycode::A)) { delta -= right; }
    if (K.keyDown(idk::Keycode::D)) { delta += right; }
    if (K.keyDown(idk::Keycode::W)) { delta += front; }
    if (K.keyDown(idk::Keycode::S)) { delta -= front; }

    motion.run    = K.keyDown(idk::Keycode::LSHIFT);
    motion.jump   = K.keyTapped(idk::Keycode::SPACE);
    motion.crouch = K.keyDown(idk::Keycode::LCTRL);
    motion.aim    = events.mouseDown(idk::MouseButton::RIGHT);
    motion.attack = events.mouseClicked(idk::MouseButton::LEFT);

    motion.delta = delta;
    motion.yaw   = dmouse.x;
    motion.pitch = dmouse.y;

    return motion;
}


meatworld::Player::Player( const glm::vec3 &position, float yaw )
:   meatworld::Humanoid( position, yaw )
{
    std::cout << "Player::Player\n";

    m_cam_obj    = ECS2::createGameObject("camera", false);
    m_hinge_obj  = ECS2::createGameObject("hinge",  false);

    ECS2::giveChild(m_hinge_obj, m_cam_obj);

    ECS2::giveComponent<PlayerCmp>(m_root_obj);
    ECS2::giveComponent<AudioListenerCmp>(m_root_obj);
    ECS2::giveComponent<SmoothFollowCmp>(m_hinge_obj);
    ECS2::giveComponent<CameraCmp>(m_cam_obj);

    TransformSys::getLocalPosition(m_cam_obj) = glm::vec3(0.5f, 0.0f, 1.25f);
    TransformSys::getTransformCmp(m_cam_obj).yaw = 0.0f;
    TransformSys::getTransform(m_cam_obj).rotation = glm::quat(glm::vec3(0.0f));

}


meatworld::Player::~Player()
{
    ECS2::deleteGameObject(m_hinge_obj);
}


void
meatworld::Player::move( idk::EngineAPI &api, const CharacterMotion &motion )
{
    using namespace idk;

    float dt = api.dtime();

    ECS2::getComponent<SmoothFollowCmp>(m_hinge_obj).anchor_id = m_head_obj;
    ECS2::getComponent<SmoothFollowCmp>(m_hinge_obj).speed     = 16.0f;

    {
        TransformSys::pitch(m_hinge_obj, motion.pitch);
        TransformSys::yaw(m_hinge_obj, motion.yaw);
    }

    {
        float &A = TransformSys::getTransformCmp(m_root_obj).yaw;
        float  B = TransformSys::getTransformCmp(m_hinge_obj).yaw;

        float alpha = 0.0f;

        if (motion.aim)
        {
            alpha = 0.25f;
        }

        else if (glm::length2(motion.delta) > 0.0f)
        {
            alpha = 0.05f;
        }

        A = idk::mixRadians(A, B, alpha);


        float &C = TransformSys::getTransformCmp(m_head_obj).pitch;
        float  D = TransformSys::getTransformCmp(m_hinge_obj).pitch;
        C = idk::mixRadians(C, D, 0.1f);

    }


    if (motion.aim)
    {
        glm::vec3 pos   = TransformSys::getWorldPosition(m_cam_obj);
        glm::vec3 front = 4.0f * TransformSys::getFront(m_cam_obj);

        reachFor(pos+front);
    }


    Humanoid::move(api, motion);

    // if (motion.aim)
    // {
    //     auto &cmp = ECS2::getComponent<CameraCmp>(m_cam_obj);
    //     glm::vec3 &pos = TransformSys::getTransform(m_cam_obj).position;

    //     TransformSys::getTransformCmp(m_weapon_obj).pitch = TransformSys::getTransformCmp(m_hinge_obj).pitch;

    //     if (cmp.camera.fov_offset > -25.0f)
    //     {
    //         cmp.camera.fov_offset -= 200.0f * dt;
    //     }

    //     // if (pos.x > m_arms[1]->getOffset().x + 0.25)
    //     // {
    //     //     pos.x -= 0.05f;
    //     // }

    //     if (pos.z > 0.5f)
    //     {
    //         pos.z -= 0.05f;
    //     }
    // }

    // else
    // {
    //     auto &cmp = ECS2::getComponent<CameraCmp>(m_cam_obj);
    //     glm::vec3 &pos = TransformSys::getTransform(m_cam_obj).position;
    //     // TransformSys::getTransformCmp(m_weapon_obj).pitch = glm::radians(-45.0f);
    //     // TransformSys::getTransformCmp(m_weapon_obj).yaw   = glm::radians(16.0f);

    //     if (cmp.camera.fov_offset < 0.0f)
    //     {
    //         cmp.camera.fov_offset += 200.0f * dt;
    //     }

    //     if (pos.x < 0.5f)
    //     {
    //         pos.x += 0.05f;
    //     }

    //     if (pos.z < 1.25f)
    //     {
    //         pos.z += 0.05f;
    //     }
    // }

}



// meatworld::Player::~Player()
// {
//     // ECS2::deleteGameObject(m_cam_obj);
// }


// meatworld::Player::Player()
// {
//     using namespace idk;

//     m_obj_id = ECS2::createGameObject("Player", false);
//     ECS2::giveComponent<TransformCmp>(m_obj_id);
//     ECS2::giveComponent<AudioListenerCmp>(m_obj_id);
//     TransformSys::setPositionLocalspace(m_obj_id, glm::vec3(0.0f));



//     m_torso_obj = ECS2::createGameObject("torso", false);
//     m_head_obj  = ECS2::createGameObject("head", false);

//     ECS2::giveComponent<TransformCmp>(m_torso_obj);
//     ECS2::giveComponent<TransformCmp>(m_head_obj);
//     ECS2::giveComponent<ModelCmp>(m_torso_obj);
//     ECS2::giveComponent<ModelCmp>(m_head_obj);

//     ECS2::giveChild(m_obj_id, m_torso_obj);
//     ECS2::giveChild(m_torso_obj, m_head_obj);

//     TransformSys::setPositionLocalspace(m_torso_obj, glm::vec3(0.0f));
//     TransformSys::setPositionLocalspace(m_head_obj, glm::vec3(0.0f, 1.0f, 0.0f));

//     TransformSys::getUniformScale(m_torso_obj) = 0.2f;
//     TransformSys::getUniformScale(m_head_obj)  = 0.2f;

//     ModelSys::assignModel(m_torso_obj, "assets/models/npc/meat-torso.idkvi");
//     ModelSys::assignModel(m_head_obj,  "assets/models/npc/meat-head.idkvi");




//     if (m_audio_emitter == -1)
//     {
//         int sound = AudioSystem::loadWav("assets/audio/footsteps.wav");
//         m_audio_emitter = AudioSystem::createEmitter(AudioSystem::Emitter(sound));
//     }

//     m_cam_obj = ECS2::createGameObject("camera", false);
//     ECS2::giveComponent<TransformCmp>(m_cam_obj);
//     ECS2::giveComponent<CameraCmp>(m_cam_obj);

//     TransformSys::getLocalPosition(m_cam_obj) = glm::vec3(0.5f, 0.5f, 0.7f);
//     TransformSys::getTransformCmp(m_cam_obj).yaw = 0.0f;
//     TransformSys::getTransform(m_cam_obj).rotation = glm::quat(glm::vec3(0.0f));

//     m_hit_obj = ECS2::createGameObject("hitbox", false);
//     ECS2::giveComponent<CharacterHitBoxCmp>(m_hit_obj);
//     TransformSys::getLocalPosition(m_hit_obj) = glm::vec3(0.0f, -0.11f, 0.08f);
//     TransformSys::getXYZScale(m_hit_obj) = glm::vec3(0.5f, 0.481f, 0.33f);

//     auto &cmp = ECS2::getComponent<CharacterHitBoxCmp>(m_hit_obj);
//     cmp.callback = [this]()
//     {
//         m_health -= 1;
//         std::cout << "Health: " << m_health << "\n";
//     };


//     ECS2::giveChild(m_obj_id, m_cam_obj);
//     ECS2::giveChild(m_obj_id, m_hit_obj);

//     m_flashlight.init(m_cam_obj);

// }


// meatworld::Player::~Player()
// {
//     using namespace idk;
//     std::cout << "Player::~Player\n";
//     ECS2::deleteGameObject(m_obj_id);
// }




// void
// meatworld::Player::move( idk::EngineAPI &api, const CharacterMotion &motion )
// {
//     using namespace idk;

//     float dtime  = api.dtime();
//     auto &ren    = api.getRenderer();
    
//     auto &events = api.getEventSys();
//     auto &K      = events.keylog();


//     // glm::vec3 up    = TransformSys::getUp(m_obj_id);
//     // glm::vec3 right = TransformSys::getRight(m_cam_obj);
//     // glm::vec3 front = TransformSys::getFront(m_cam_obj);

//     glm::mat4 T = TransformSys::getModelMatrix(m_obj_id);
//     glm::vec3 delta = glm::mat3(T) * motion.delta;


//     auto &cmp = ECS2::getComponent<KinematicCapsuleCmp>(m_obj_id);
//     PhysicsSys::addForce(m_obj_id, delta);

//     if (motion.jump)
//     {
//         PhysicsSys::jump(m_obj_id, m_jump_force);
//     }

//     if (motion.crouch)
//     {
//         cmp.crouch = true;
//     }

//     TransformSys::pitch(m_cam_obj, motion.pitch);
//     TransformSys::yaw(m_obj_id, motion.yaw);

//     TransformSys::roll(m_cam_obj, 0.05f * motion.yaw);
//     TransformSys::getTransformCmp(m_cam_obj).roll *= 0.98f;
// };



// void
// meatworld::Player::update( idk::EngineAPI &api )
// {
//     using namespace idk;

//     float dt  = api.dtime();
//     auto &ren = api.getRenderer();
    
//     auto &events = api.getEventSys();
//     auto &K      = events.keylog();

//     if (events.mouseCaptured() == false)
//     {
//         return;
//     }

//     if (K.keyTapped(idk::Keycode::F))
//     {
//         m_flashlight.toggle();
//     }


//     // this->move(api, delta, dmouse.x, dmouse.y);


//     if (K.keyTapped(Keycode::N1))
//     {
//         giveWeapon<Glock>();
//     }

//     else if (K.keyTapped(Keycode::N2))
//     {
//         giveWeapon<HL2_AR2>();
//     }

//     // if (m_weapon)
//     // {
//     //     m_weapon->update(api, dmouse.x, dmouse.y);
    
//     //     if (events.mouseClicked(MouseButton::LEFT))
//     //     {
//     //         m_weapon->attack(api);
//     //     }
//     // }

// };



// void
// meatworld::Player::update( idk::EngineAPI &api, meatnet::PeerData &data )
// {
//     using namespace idk;

//     int obj_id = m_obj_id;

//     auto &pcmp = TransformSys::getTransformCmp(obj_id);
//     auto &ccmp = TransformSys::getTransformCmp(m_cam_obj);

//     data.position = TransformSys::getWorldPosition(obj_id);
//     data.pitch = uint8_t(255.0f * ccmp.pitch / (2.0 * M_PI));
//     data.yaw   = uint8_t(255.0f * pcmp.yaw   / (2.0 * M_PI));

//     uint32_t action = 0;

//     if (api.getEventSys().mouseClicked(MouseButton::LEFT))
//     {
//         action |= meatnet::ACTION_SHOOT;
//     }


//     uint32_t state = 0;

//     if (m_flashlight.m_on)
//     {
//         state |= meatnet::STATE_FLASHLIGHT;
//     }


//     uint32_t weapon = 0;

//     if (m_weapon)
//     {
//         if (m_weapon->getDesc().name == "Glock")
//         {
//             weapon |= meatnet::WEAPON_GLOCK;
//         }

//         else if (m_weapon->getDesc().name == "AR2")
//         {
//             weapon |= meatnet::WEAPON_AR2;
//         }
//     }


//     data.action = action;
//     data.state  = state;
//     data.weapon = weapon;

// }




// void
// meatworld::EditorPlayer::update( idk::EngineAPI &api )
// {
//     using namespace idk;

//     float dtime  = api.dtime();
//     auto &ren    = api.getRenderer();
    
//     auto &events = api.getEventSys();
//     auto &K      = events.keylog();

//     if (events.mouseCaptured() == false)
//     {
//         return;
//     }

//     glm::vec3 delta = glm::vec3(0.0f);
//     glm::vec3 up    = TransformSys::getUp(m_obj_id);
//     glm::vec3 right = TransformSys::getRight(m_cam_obj);
//     glm::vec3 front = TransformSys::getFront(m_cam_obj);


//     if (K.keyDown(idk::Keycode::A)) { delta -= right; }
//     if (K.keyDown(idk::Keycode::D)) { delta += right; }
//     if (K.keyDown(idk::Keycode::W)) { delta += front; }
//     if (K.keyDown(idk::Keycode::S)) { delta -= front; }

//     delta.y = 0.0f;

//     if (K.keyDown(idk::Keycode::SPACE)) { delta += up; }
//     if (K.keyDown(idk::Keycode::LCTRL)) { delta -= up; }

//     if (fabs(delta.x) > 0.01f|| fabs(delta.y) > 0.01f || fabs(delta.z) > 0.01f)
//     {
//         delta = 0.0025f * m_walk_speed * glm::normalize(delta);
//     }


//     TransformSys::translateWorldspace(m_obj_id, delta);

//     glm::vec2 dmouse = 0.1f * events.mouseDelta();
//     TransformSys::pitch(m_cam_obj, -dmouse.y);
//     TransformSys::yaw(m_obj_id, -dmouse.x);
// };