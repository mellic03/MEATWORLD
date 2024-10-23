#include "player.hpp"

#include <IDKAudio/IDKAudio.hpp>
#include <IDKBuiltinCS/sys-audio.hpp>

#include <libidk/idk_math.hpp>

#include "../systems/sys-player.hpp"
#include "player-fps.hpp"



meatworld::PlayerFPS::PlayerFPS( const glm::vec3 &position, float yaw )
:   CharacterBase("Player", position, yaw)
{
    using namespace idk;

    // m_root_obj = ECS2::createGameObject("player", false);
    m_cam_obj  = ECS2::createGameObject("camera", false);

    ECS2::giveChild(m_root_obj, m_cam_obj);
    ECS2::giveComponent<PlayerCmp>(m_root_obj);
    ECS2::giveComponent<AudioListenerCmp>(m_root_obj);
    ECS2::giveComponent<CameraCmp>(m_cam_obj);

    TransformSys::getLocalPosition(m_cam_obj) *= 0.0f;
    // TransformSys::setWorldPosition(m_root_obj, position);
    // TransformSys::getTransformCmp(m_root_obj).yaw = yaw;

    ECS2::giveComponent<KinematicCapsuleCmp>(m_root_obj);
    auto &cmp = ECS2::getComponent<KinematicCapsuleCmp>(m_root_obj);

    cmp.curr_pos = position;
    cmp.prev_pos = cmp.curr_pos;
    cmp.bottom = 1.5f;
}


meatworld::PlayerFPS::~PlayerFPS()
{
    // ECS2::deleteGameObject(m_hinge_obj);
}


void
meatworld::PlayerFPS::move( idk::EngineAPI &api, const CharacterMotion &motion )
{
    using namespace idk;

    float dtime = api.dtime();
    float &fov  = ECS2::getComponent<CameraCmp>(m_cam_obj).camera.fov_offset;

    if (m_weapon)
    {
        m_weapon->aim(motion.aim);

        if (motion.attack)
        {
            m_weapon->attack(api);
        }

        m_weapon->update(api, motion.yaw, motion.pitch, &fov);
    }


    glm::mat3 R = glm::mat3(TransformSys::getModelMatrix(m_cam_obj));


    auto &cmp = ECS2::getComponent<KinematicCapsuleCmp>(m_root_obj);

    if (cmp.enabled)
    {
        glm::vec3 delta = motion.delta * glm::vec3(1, 0, 1);
        if (fabs(delta.x) > 0.00001f && fabs(delta.z) > 0.00001f)
        {
            delta = glm::length(motion.delta) * glm::normalize(delta);
        }

        PhysicsSys::addForce(m_root_obj, R * (4.0f * delta));
    }

    else
    {
        TransformSys::translateWorldspace(m_root_obj, R * (dtime * 8.0f * motion.delta));
    }

    TransformSys::yaw(m_root_obj, motion.yaw);
    TransformSys::pitch(m_cam_obj, motion.pitch);

    float &roll = TransformSys::getTransformCmp(m_cam_obj).roll;

    roll -= 0.002f * motion.yaw;
    roll += 0.002f * motion.delta.x;
    // roll = glm::mix(roll, roll + motion.yaw, 0.0f);

    roll *= 0.96f;

}


void
meatworld::PlayerFPS::update( idk::EngineAPI &api )
{


}