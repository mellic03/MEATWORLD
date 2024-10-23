#include "flashlight.hpp"
#include "../world/world.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKIO/IDKIO.hpp>

using namespace meat;


ItemFlashlight::ItemFlashlight( ActorBase *owner )
:   ItemBase (owner),
    m_obj    (idk::ECS2::createGameObject("Flashlight", false))
{
    idk::ECS2::giveChild(owner->getAttachID(), m_obj);
    idk::TransformSys::getLocalPosition(m_obj) = glm::vec3(0.0f, 0.0f, -0.05f);

    idk::ECS2::giveComponent<idk::SpotlightCmp>(m_obj);
    idk::ECS2::getComponent<idk::SpotlightCmp>(m_obj).light.diffuse = glm::vec4(0.0f);

    idk::ECS2::giveComponent<idk::AudioEmitterCmp>(m_obj);
    idk::AudioSys::assignSound(m_obj, "assets/audio/light-switch.wav");
}


ItemFlashlight::~ItemFlashlight()
{
    idk::ECS2::deleteGameObject(m_obj);
}



void
ItemFlashlight::on()
{
    auto &cmp = idk::ECS2::getComponent<idk::SpotlightCmp>(m_obj);
    cmp.light.diffuse = glm::vec4(1.0f);

    m_enabled = true;
    idk::AudioSys::playSound(m_obj, false);
}


void
ItemFlashlight::off()
{
    auto &cmp = idk::ECS2::getComponent<idk::SpotlightCmp>(m_obj);
    cmp.light.diffuse = glm::vec4(0.0f);

    m_enabled = false;
    idk::AudioSys::playSound(m_obj, false);
}


void
ItemFlashlight::toggle()
{
    if (m_enabled)
    {
        off();
    }

    else
    {
        on();
    }
}



void
ItemFlashlight::update( idk::EngineAPI &api, meat::World &world )
{
    if (idkio::keyTapped(idk::Keycode::F))
    {
        toggle();
    }
}

