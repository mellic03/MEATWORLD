#include "player-tps.hpp"

#include <IDKAudio/IDKAudio.hpp>
#include <IDKBuiltinCS/sys-audio.hpp>

#include "../systems/sys-grabbable.hpp"
#include "../systems/sys-weapon.hpp"

#include "../game.hpp"
#include "../../../meatnet/src/meatnet.hpp"




meatworld::PlayerTPS::PlayerTPS()
{
    using namespace idk;

    m_obj_id = ECS2::createGameObject("PlayerTPS", false);
    ECS2::giveComponent<TransformCmp>(m_obj_id);
    ECS2::giveComponent<AudioListenerCmp>(m_obj_id);
    TransformSys::setPositionLocalspace(m_obj_id, glm::vec3(0.0f));

    if (m_audio_emitter == -1)
    {
        int sound = AudioSystem::loadWav("assets/audio/footsteps.wav");
        m_audio_emitter = AudioSystem::createEmitter(AudioSystem::Emitter(sound));
    }

    m_cam_obj = ECS2::createGameObject("camera", false);
    ECS2::giveComponent<TransformCmp>(m_cam_obj);
    ECS2::giveComponent<CameraCmp>(m_cam_obj);

    TransformSys::getLocalPosition(m_cam_obj) = glm::vec3(0.0f);
    TransformSys::getTransformCmp(m_cam_obj).yaw = 0.0f;
    TransformSys::getTransform(m_cam_obj).rotation = glm::quat(glm::vec3(0.0f));

    m_hit_obj = ECS2::createGameObject("hitbox", false);
    ECS2::giveComponent<CharacterHitBoxCmp>(m_hit_obj);
    TransformSys::getLocalPosition(m_hit_obj) = glm::vec3(0.0f, -0.11f, 0.08f);
    TransformSys::getXYZScale(m_hit_obj) = glm::vec3(0.5f, 0.481f, 0.33f);


    auto &cmp = ECS2::getComponent<CharacterHitBoxCmp>(m_hit_obj);
    cmp.callback = [this]()
    {
        m_health -= 1;
        std::cout << "Health: " << m_health << "\n";
    };


    ECS2::giveChild(m_obj_id, m_cam_obj);
    ECS2::giveChild(m_obj_id, m_hit_obj);

    m_flashlight.init(m_cam_obj);

}


meatworld::PlayerTPS::~PlayerTPS()
{
    using namespace idk;
    std::cout << "PlayerTPS::~PlayerTPS\n";
    ECS2::deleteGameObject(m_obj_id);
}




void
meatworld::PlayerTPS::move( idk::EngineAPI &api, const glm::vec3 &delta, float dx, float dy )
{
    using namespace idk;

    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();


    auto &cmp = ECS2::getComponent<KinematicCapsuleCmp>(m_obj_id);

    PhysicsSys::addForce(m_obj_id, delta);

    if (K.keyTapped(idk::Keycode::SPACE))
    {
        PhysicsSys::jump(m_obj_id, m_jump_force);
    }

    cmp.crouch = K.keyDown(idk::Keycode::LCTRL);

    TransformSys::pitch(m_cam_obj, dy);
    TransformSys::yaw(m_obj_id, dx);

    TransformSys::roll(m_cam_obj, 0.05f * dx);
    TransformSys::getTransformCmp(m_cam_obj).roll *= 0.98f;
};



void
meatworld::PlayerTPS::update( idk::EngineAPI &api )
{
    using namespace idk;

    float dt  = api.getEngine().deltaTime();
    auto &ren = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    if (events.mouseCaptured() == false)
    {
        return;
    }


    if (K.keyTapped(idk::Keycode::F))
    {
        m_flashlight.toggle();
    }



    glm::vec3 delta = glm::vec3(0.0f);
    glm::vec3 up    = TransformSys::getUp(m_obj_id);
    glm::vec3 right = TransformSys::getRight(m_cam_obj);
    glm::vec3 front = TransformSys::getFront(m_cam_obj);

    static float walk_roll = 0.0f;
    walk_roll *= 0.98f;
    TransformSys::getTransformCmp(m_obj_id).roll = walk_roll;


    float curr_speed = m_walk_speed;

    if (K.keyDown(idk::Keycode::LSHIFT)) { curr_speed = m_run_speed; };
    if (K.keyDown(idk::Keycode::A)) { delta -= right; walk_roll -= 0.001f; }
    if (K.keyDown(idk::Keycode::D)) { delta += right; walk_roll += 0.001f; }
    if (K.keyDown(idk::Keycode::W)) { delta += front; }
    if (K.keyDown(idk::Keycode::S)) { delta -= front; };

    delta.y = 0.0f;

    if (glm::length2(delta) > 0.0f)
    {
        AudioSystem::playSound(m_audio_emitter, true);
    }

    else
    {
        AudioSystem::stopSound(m_audio_emitter);
    }



    if (fabs(delta.x) > 0.01f || fabs(delta.z) > 0.01f)
    {
        delta = curr_speed * glm::normalize(delta);
    }

    glm::vec2 dmouse = -0.1f * events.mouseDelta();

    this->move(api, delta, dmouse.x, dmouse.y);


    if (K.keyTapped(Keycode::N1))
    {
        giveWeapon<Glock>();
    }

    else if (K.keyTapped(Keycode::N2))
    {
        giveWeapon<HL2_AR2>();
    }

    if (m_weapon)
    {
        m_weapon->update(api, dmouse.x, dmouse.y);
    
        if (events.mouseClicked(MouseButton::LEFT))
        {
            m_weapon->attack(api);
        }
    }


};



void
meatworld::PlayerTPS::update( idk::EngineAPI &api, meatnet::PeerData &data )
{
    using namespace idk;

    int obj_id = m_obj_id;

    auto &pcmp = TransformSys::getTransformCmp(obj_id);
    auto &ccmp = TransformSys::getTransformCmp(m_cam_obj);

    data.position = TransformSys::getWorldPosition(obj_id);
    data.pitch = uint8_t(255.0f * ccmp.pitch / (2.0 * M_PI));
    data.yaw   = uint8_t(255.0f * pcmp.yaw   / (2.0 * M_PI));

    uint32_t action = 0;

    if (api.getEventSys().mouseClicked(MouseButton::LEFT))
    {
        action |= meatnet::ACTION_SHOOT;
    }


    uint32_t state = 0;

    if (m_flashlight.m_on)
    {
        state |= meatnet::STATE_FLASHLIGHT;
    }


    uint32_t weapon = 0;

    if (m_weapon)
    {
        if (m_weapon->getDesc().name == "Glock")
        {
            weapon |= meatnet::WEAPON_GLOCK;
        }

        else if (m_weapon->getDesc().name == "AR2")
        {
            weapon |= meatnet::WEAPON_AR2;
        }
    }


    data.action = action;
    data.state  = state;
    data.weapon = weapon;

}

