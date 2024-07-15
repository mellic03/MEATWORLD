#include "player.hpp"

#include "../systems/sys-grabbable.hpp"
#include "../systems/sys-weapon.hpp"
#include "../../../meatnet/src/meatnet.hpp"




meatworld::Player::Player()
{
    using namespace idk;

    m_obj_id = ECS2::createGameObject("Player", false);
    ECS2::giveComponent<TransformCmp>(m_obj_id);
    ECS2::giveComponent<AudioListenerCmp>(m_obj_id);
    TransformSys::setPositionLocalspace(m_obj_id, glm::vec3(0.0f));

    m_cam_obj = ECS2::createGameObject("camera", false);
    ECS2::giveComponent<TransformCmp>(m_cam_obj);
    ECS2::giveComponent<CameraCmp>(m_cam_obj);

    TransformSys::getLocalPosition(m_cam_obj) = glm::vec3(0.0f);
    TransformSys::getTransformCmp(m_cam_obj).yaw = 0.0f;
    TransformSys::getTransform(m_cam_obj).rotation = glm::quat(glm::vec3(0.0f));

    m_hit_obj = ECS2::createGameObject("hitbox", false);
    ECS2::giveComponent<CharacterHitBoxCmp>(m_hit_obj);


    auto &cmp = ECS2::getComponent<CharacterHitBoxCmp>(m_hit_obj);
    cmp.callback = [this]()
    {
        m_health -= 1;
        std::cout << "Health: " << m_health << "\n";
    };



    ECS2::giveChild(m_obj_id, m_cam_obj);
    ECS2::giveChild(m_obj_id, m_hit_obj);

    m_flashlight.init(m_obj_id);

}


meatworld::Player::~Player()
{
    using namespace idk;
    std::cout << "Player::~Player\n";
    ECS2::deleteGameObject(m_obj_id);
}




void
meatworld::Player::update( idk::EngineAPI &api )
{
    using namespace idk;

    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    
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

    float curr_speed = m_walk_speed;

    if (K.keyDown(idk::Keycode::LSHIFT)) { curr_speed = m_run_speed; };

    if (K.keyDown(idk::Keycode::A)) { delta -= right; walk_roll -= 0.001f; }
    if (K.keyDown(idk::Keycode::D)) { delta += right; walk_roll += 0.001f; }
    if (K.keyDown(idk::Keycode::W)) { delta += front; }
    if (K.keyDown(idk::Keycode::S)) { delta -= front; }

    TransformSys::getTransformCmp(m_obj_id).roll = walk_roll;
    walk_roll *= 0.98f;


    delta.y = 0.0f;

    if (fabs(delta.x) > 0.01f || fabs(delta.z) > 0.01f)
    {
        delta = glm::normalize(delta);
    }


    auto &cmp = ECS2::getComponent<KinematicCapsuleCmp>(m_obj_id);

    if (cmp.enabled)
    {
        PhysicsSys::addForce(m_obj_id, curr_speed * delta);

        if (K.keyTapped(idk::Keycode::SPACE))
        {
            PhysicsSys::jump(m_obj_id, m_jump_force);
        }

        cmp.crouch = K.keyDown(idk::Keycode::LCTRL);
    }

    else
    {
        TransformSys::translateWorldspace(m_obj_id, delta);
    }


    glm::vec2 dmouse = 0.1f * events.mouseDelta();

    TransformSys::pitch(m_cam_obj, -dmouse.y);
    TransformSys::yaw(m_obj_id, -dmouse.x);

    TransformSys::roll(m_cam_obj, -0.05f * dmouse.x);
    TransformSys::getTransformCmp(m_cam_obj).roll *= 0.98f;


    if (K.keyTapped(Keycode::N1))
    {
        if (m_weapon)
        {
            delete m_weapon;
        }

        m_weapon = new Glock(m_cam_obj, m_hit_obj);
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
meatworld::Player::update( idk::EngineAPI &api, meatnet::PeerData &data )
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






void
meatworld::EditorPlayer::update( idk::EngineAPI &api )
{
    using namespace idk;

    float dtime  = api.getEngine().deltaTime();
    auto &ren    = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();

    if (events.mouseCaptured() == false)
    {
        return;
    }

    glm::vec3 delta = glm::vec3(0.0f);
    glm::vec3 up    = TransformSys::getUp(m_obj_id);
    glm::vec3 right = TransformSys::getRight(m_cam_obj);
    glm::vec3 front = TransformSys::getFront(m_cam_obj);


    if (K.keyDown(idk::Keycode::A)) { delta -= right; }
    if (K.keyDown(idk::Keycode::D)) { delta += right; }
    if (K.keyDown(idk::Keycode::W)) { delta += front; }
    if (K.keyDown(idk::Keycode::S)) { delta -= front; }

    delta.y = 0.0f;

    if (K.keyDown(idk::Keycode::SPACE)) { delta += up; }
    if (K.keyDown(idk::Keycode::LCTRL)) { delta -= up; }

    if (fabs(delta.x) > 0.01f|| fabs(delta.y) > 0.01f || fabs(delta.z) > 0.01f)
    {
        delta = 0.0025f * m_walk_speed * glm::normalize(delta);
    }


    TransformSys::translateWorldspace(m_obj_id, delta);

    glm::vec2 dmouse = 0.1f * events.mouseDelta();
    TransformSys::pitch(m_cam_obj, -dmouse.y);
    TransformSys::yaw(m_obj_id, -dmouse.x);
};