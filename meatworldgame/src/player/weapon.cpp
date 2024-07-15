#include "weapon.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-audio.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>

#include "../systems/sys-weapon.hpp"


meatworld::WeaponBase::WeaponBase( const std::string &name, int parent )
{
    using namespace idk;


}



meatworld::WeaponBase::~WeaponBase()
{
    using namespace idk;

    if (ECS2::gameObjectExists(m_obj_id))
    {
        ECS2::deleteGameObject(m_obj_id);
    }

}



meatworld::RangedWeapon::RangedWeapon( int parent, const WeaponDesc &desc, int ignore )
:   WeaponBase (desc.name, parent),
    m_desc     (desc)
{
    using namespace idk;

    m_weapon_obj = ECS2::createGameObject("model", false);
    ECS2::giveComponent<AudioEmitterCmp>(m_weapon_obj);
    ECS2::giveComponent<ModelCmp>(m_weapon_obj);
    ECS2::giveComponent<WeaponCmp>(m_weapon_obj);
    ECS2::getComponent<WeaponCmp>(m_weapon_obj).ignore_obj = ignore;

    WeaponSys::config(m_weapon_obj, {m_desc.spread, m_desc.shots});

    AudioSys::assignSound(m_weapon_obj, m_desc.audio_path);
    ModelSys::assignModel(m_weapon_obj, m_desc.model_path);

    m_obj_id = ECS2::createGameObject(m_desc.name, false);
    ECS2::giveChild(parent, m_obj_id);
    ECS2::giveChild(m_obj_id, m_weapon_obj);

    {
        glm::vec3 &position = TransformSys::getLocalPosition(m_obj_id);
                   position = glm::vec3(0.0f, 0.0f, -0.2f);
    }

    {
        glm::vec3 &position = TransformSys::getLocalPosition(m_weapon_obj);
                   position = m_desc.rest - glm::vec3(0.0f, 0.1f, 0.0f);
    }

}





void
meatworld::RangedWeapon::attack( idk::EngineAPI &api )
{
    idk::TransformSys::translateLocalspace(m_weapon_obj, glm::vec3(0.0f, 0.0f, 0.2f * m_desc.recoil));
    idk::AudioSys::playSound(m_weapon_obj, false);
    WeaponSys::fire(m_weapon_obj);
}



void
meatworld::RangedWeapon::update( idk::EngineAPI &api, float dx, float dy )
{
    using namespace idk;

    glm::vec3 target = m_desc.rest;
    glm::vec3 speed  = m_desc.aim_speed;

    if (api.getEventSys().mouseDown(MouseButton::RIGHT))
    {
        target = m_desc.aim;
    }

    glm::vec3 &position = TransformSys::getLocalPosition(m_weapon_obj);

    position.x -= m_desc.sway_speed.x * dx;
    position.y += m_desc.sway_speed.y * dy;

    glm::vec3 dir = target - position;
    position += speed * dir;

    // position.x *= m_desc.rest_speed.x;
    // position.y *= m_desc.rest_speed.y;

}





