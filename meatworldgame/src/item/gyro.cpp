#include "gyro.hpp"
#include <libidk/idk_random.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/particle/particle.hpp>

using namespace meat;


ItemGyro::ItemGyro( ActorBase *owner )
:   ItemBase(owner)
{

}


void
ItemGyro::update( idk::EngineAPI &api, meat::World& )
{
    float dt = api.dtime();
    m_accum += dt;

    while (m_accum > m_tickrate)
    {
        glm::vec3 front = idk::TransformSys::getFront(m_owner->getID());
        glm::vec3 up    = idk::TransformSys::getUp(m_owner->getID());

        m_reading  = glm::dot(up, glm::vec3(0, 1, 0));
        m_reading += idk::randf(-0.05f, +0.05f);
        m_ready    = true;

        m_accum -= m_tickrate;
    }

}


bool
ItemGyro::read( float *f )
{
    if (m_ready)
    {
        *f = m_reading;
        m_ready = false;
        return true;
    }

    return false;
}





