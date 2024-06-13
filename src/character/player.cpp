#include "character.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKEvents/IDKEvents.hpp>

#include <libidk/idk_log.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>


idkg::Player::Player( idk::EngineAPI &api, int obj_id )
:   idkg::Character(api, obj_id)
{
    // auto &ecs = api.getECS();
    // auto &ren = api.getRenderer();


    // m_hinge_obj = ecs.createGameObject("Hinge");
    // ecs.giveComponent<idk::TransformCmp>(m_hinge_obj);

    // m_cam_obj = ecs.createGameObject("Camera");
    // ecs.giveComponent<idk::TransformCmp>(m_cam_obj);

    // int cam = ren.createCamera();
    // ren.useCamera(cam);

    // ecs.giveComponent<idk::CameraCmp>(m_cam_obj);
    // ecs.getComponent<idk::CameraCmp>(m_cam_obj).cam_id = cam;


    // ecs.giveChild(m_obj, m_hinge_obj);
    // ecs.giveChild(m_hinge_obj, m_cam_obj);

    // idk::TransformSys::translateLocalspace(m_cam_obj, glm::vec3(0.0f, 0.0f, 3.0f));


    // ecs.giveComponent<idk::KinematicCapsuleCmp>(m_obj);
    // auto &cmp = ecs.getComponent<idk::KinematicCapsuleCmp>(m_obj);
}



void
idkg::Player::update( idk::EngineAPI &api )
{
    // float dtime  = api.getEngine().deltaTime();
    // auto &ren    = api.getRenderer();
    // auto &ecs    = api.getECS();
    // auto &events = api.getEventSys();
    // auto &K      = events.keylog();

    // auto &T = ecs.getComponent<idk::TransformCmp>(m_obj);


    // glm::vec3 delta = glm::vec3(0.0f);
    // float     speed = 8.0f;

    // glm::vec3 up    = dtime * speed * idk::TransformSys::getUp(m_hinge_obj);
    // glm::vec3 right = dtime * speed * idk::TransformSys::getRight(m_hinge_obj);
    // glm::vec3 front = dtime * speed * idk::TransformSys::getFront(m_hinge_obj);


    // static float travel = 0.0f;
    // glm::vec3 left_foot  = T.position - glm::normalize(right);
    // glm::vec3 right_foot = T.position + glm::normalize(right);

    // left_foot.y  += 0.25f*sin(travel);
    // right_foot.y += 0.25f*sin(-travel);

    // ren.drawSphere(left_foot, 0.1f);
    // ren.drawSphere(right_foot, 0.1f);


    // if (K.keyDown(idk::Keycode::A)) delta -= right;
    // if (K.keyDown(idk::Keycode::D)) delta += right;
    // if (K.keyDown(idk::Keycode::W)) delta += front;
    // if (K.keyDown(idk::Keycode::S)) delta -= front;

    // delta.y = 0.0f;
    // travel += glm::length(delta);

    // idk::TransformSys::translateLocalspace(m_obj, delta);


    // static float G = 3.0f*9.8f;
    // static glm::vec3 acc = glm::vec3(0.0f, 0.0f, 0.0f);
    // static glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f);

    // auto &cmp = ecs.getComponent<idk::KinematicCapsuleCmp>(m_obj);

    // glm::vec3 prev_vel = vel;

    // if (cmp.grounded)
    // {
    //     acc.y *= 0.75f;
    //     vel.y *= 0.75f;

    //     if (K.keyDown(idk::Keycode::SPACE))
    //     {
    //         acc.y = 0.2f*G;
    //         vel.y = 0.4f*G;
    //     }
    // }

    // else
    // {
    //     vel = vel + dtime*acc;
    //     acc.y -= dtime * G;
    //     vel += dtime * G * acc;
    // }

    // // acc *= 0.95f;
    // // vel *= 0.995f;

    // acc = glm::clamp(acc, glm::vec3(-25.0f), glm::vec3(25.0f));
    // vel = glm::clamp(vel, glm::vec3(-25.0f), glm::vec3(25.0f));


    // idkui::TextManager::text(100, 150) << "acc: " << acc.y;
    // idkui::TextManager::text(100, 175) << "vel: " << vel.y;

    // idk::TransformSys::translateWorldspace(m_obj, 0.5f * (vel + prev_vel) * dtime);


    // if (K.keyTapped(idk::Keycode::ESCAPE))
    // {
    //     bool captured = events.mouseCaptured();
    //     events.mouseCapture(!captured);
    // }

    // if (events.mouseCaptured())
    // {
    //     glm::vec2 dmouse = -events.mouseDelta();
    //     float mouse_speed = 0.1f;

    //     idk::TransformSys::yaw(m_hinge_obj, mouse_speed*dmouse.x);
    //     idk::TransformSys::pitch(m_hinge_obj, mouse_speed*dmouse.y);
    // }

}
