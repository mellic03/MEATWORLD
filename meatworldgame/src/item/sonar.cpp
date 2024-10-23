#include "sonar.hpp"
#include <libidk/idk_random.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/particle/particle.hpp>
#include <IDKGraphics/terrain/terrain.hpp>

#include "../systems/sys-projectile.hpp"

using namespace meat;


ItemSonar::ItemSonar( ActorBase *owner, const glm::vec3 &dir )
:   ItemBase(owner),
    m_dir(dir),
    m_tickrate(1.0f/4.0f)
{

}


void
ItemSonar::update( idk::EngineAPI &api, meat::World& )
{
    // float dt = api.dtime();
    // m_accum += dt;

    // glm::vec3 pos = idk::TransformSys::getWorldPosition(m_owner->getID());
    // glm::vec3 vel = 8.0f * m_dir;

    // if (m_accum > m_tickrate)
    // {
    //     ProjectileSys::createProjectile(pos, vel, 0.0f,
    //         [this, pos]( idk::EngineAPI&, const glm::vec3 &hit, const glm::vec3 &N )
    //         {
    //             m_readings.push(pos.y - hit.y);
    //         }
    //     );

    //     m_accum = 0.0f;
    // }
}


bool
ItemSonar::read( float *f )
{
    auto pos = m_owner->getPosition();

    *f = pos.y - idk::TerrainRenderer::heightQuery(pos.x, pos.z);

    return true;

    // if (m_readings.size() > 0)
    // // {
    //     // *f = m_readings.front();
        // //      m_readings.pop();
    //     // return true;
    // // }
    // return false;
}





