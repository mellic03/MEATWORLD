#include "flashlight.hpp"

#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-audio.hpp>


void
meatworld::Flashlight::init( int parent )
{
    using namespace idk;

    if (m_obj_id == -1)
    {
        m_obj_id = ECS2::createGameObject("flashlight", false);
        ECS2::giveComponent<SpotlightCmp>(m_obj_id);

        auto &cmp = ECS2::getComponent<SpotlightCmp>(m_obj_id);
        cmp.light.diffuse = glm::vec4(0.0f);
        cmp.light.radius = 8.0f;
        cmp.light.angle = glm::vec3(0.7f, 0.9f, 0.0f);

        ECS2::giveChild(parent, m_obj_id);
        TransformSys::getLocalPosition(m_obj_id) = glm::vec3(0.0f);
    }

    if (m_emitter == -1)
    {
        m_emitter = ECS2::createGameObject("emitter", false);
        ECS2::giveComponent<AudioEmitterCmp>(m_emitter);
        AudioSys::assignSound(m_emitter, "assets/audio/light-switch.wav");

        ECS2::giveChild(parent, m_emitter);
        TransformSys::getLocalPosition(m_emitter) = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    m_on = false;
}


void
meatworld::Flashlight::on()
{
    using namespace idk;

    if (m_on == true)
    {
        return;
    }

    AudioSys::playSound(m_emitter, false);

    auto &cmp = ECS2::getComponent<SpotlightCmp>(m_obj_id);
    cmp.light.diffuse = glm::vec4(2.0f);
    m_on = true;
}


void
meatworld::Flashlight::off()
{
    using namespace idk;

    if (m_on == false)
    {
        return;
    }

    AudioSys::playSound(m_emitter, false);

    auto &cmp = ECS2::getComponent<SpotlightCmp>(m_obj_id);
    cmp.light.diffuse = glm::vec4(0.0f);
    m_on = false;
}


void
meatworld::Flashlight::toggle()
{
    using namespace idk;

    if (m_on)
    {
        off();
    }

    else
    {
        on();
    }
}

