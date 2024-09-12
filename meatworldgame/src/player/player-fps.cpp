#include "player.hpp"

#include <IDKAudio/IDKAudio.hpp>
#include <IDKBuiltinCS/sys-audio.hpp>

#include <libidk/idk_math.hpp>

#include "player-fps.hpp"




meatworld::PlayerFPS::PlayerFPS()
{
    // m_cam_obj    = ECS2::createGameObject("camera", false);
    // m_hinge_obj  = ECS2::createGameObject("hinge",  false);

    // ECS2::giveChild(m_hinge_obj, m_cam_obj);

    // ECS2::giveComponent<SmoothFollowCmp>(m_hinge_obj);
    // ECS2::giveComponent<CameraCmp>(m_cam_obj);

    // TransformSys::getLocalPosition(m_cam_obj) = glm::vec3(0.5f, 0.0f, 1.25f);
    // TransformSys::getTransformCmp(m_cam_obj).yaw = 0.0f;
    // TransformSys::getTransform(m_cam_obj).rotation = glm::quat(glm::vec3(0.0f));

}


meatworld::PlayerFPS::~PlayerFPS()
{
    // ECS2::deleteGameObject(m_hinge_obj);
}



void meatworld::PlayerFPS::move( idk::EngineAPI &api, const CharacterMotion &motion )
{

}