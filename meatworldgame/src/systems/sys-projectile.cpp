#include "sys-projectile.hpp"

#include <IDKGraphics/terrain/terrain.hpp>
#include <IDKThreading/IDKThreading.hpp>


void
meat::ProjectileSys::init( idk::EngineAPI &api )
{

}


void
meat::ProjectileSys::_cull( idk::EngineAPI &api )
{
    for (int idx: m_cull)
    {
        auto &p = m_backbuffer[idx];
        glm::vec3 N = idk::TerrainRenderer::slopeQuery(p.pos.x, p.pos.z);
        p.callback(api, p.pos, N);

        std::swap(m_backbuffer[idx], m_backbuffer.back());
        m_backbuffer.pop_back();
    }
}


void
meat::ProjectileSys::_update( float dt )
{
    float t = dt;
    float G;
    glm::vec3 acc;

    for (auto &p: m_backbuffer)
    {
        G = -9.8f * p.mass;

        acc = p.acc + glm::vec3(0.0f, G, 0.0f);

        p.prev0 = p.pos;
        p.pos += t*p.vel + 0.5f*t*t*acc;
        p.vel += t*acc;
    }


    m_cull.clear();

    for (int i=0; i<m_backbuffer.size(); i++)
    {
        auto &p = m_backbuffer[i];
        float h = idk::TerrainRenderer::heightQuery(p.pos.x, p.pos.z);

        if (p.pos.y < h)
        {
            m_cull.push_back(i);
        }
    }
}


void
meat::ProjectileSys::update( idk::EngineAPI &api )
{

    static const float timestep = 1.0f / 30.0f;
    static float accum = 0.0f;
    static bool  ready = true;

    auto  &ren = api.getRenderer();
    float dt   = api.dtime();

    accum += dt;

    // if (ready)
    // {
    //     ready = false;

        // idk::ThreadPool::createTask(
        //     [this]()
        //     {
                _update(accum);
            // },

            // [this, &api]()
            // {
                _cull(api);
                for (auto &p: m_queue)
                {
                    m_backbuffer.push_back(p);
                }
                m_queue.clear();

                m_frontbuffer.resize(m_backbuffer.size());
                std::copy(m_backbuffer.begin(), m_backbuffer.end(), m_frontbuffer.begin());

                accum = 0.0f;
                ready = true;
        //     }
        // );
    // }


    // while (accum >= timestep)
    // {
    //     float t = timestep;
    //     float G;
    //     glm::vec3 acc;

    //     for (auto &p: m_projectiles)
    //     {
    //         G = -9.8f * p.mass;

    //         acc = p.acc + glm::vec3(0.0f, G, 0.0f);

    //         p.prev0 = p.pos;
    //         p.pos += t*p.vel + 0.5f*t*t*acc;
    //         p.vel += t*acc;
    //     }

    //     accum -= timestep;



    //     static std::vector<int> cull;
    //     cull.clear();

    //     for (int i=0; i<m_projectiles.size(); i++)
    //     {
    //         auto &p = m_projectiles[i];
    //         float h = idk::TerrainRenderer::heightQuery(p.pos.x, p.pos.z);

    //         if (p.pos.y < h)
    //         {
    //             p.callback(api, p.pos, idk::TerrainRenderer::slopeQuery(p.pos.x, p.pos.z));
    //             cull.push_back(i);
    //         }
    //     }

    //     for (int idx: cull)
    //     {
    //         deleteProjectile(idx);
    //     }
    // }



    // float alpha = glm::clamp(accum / timestep, 0.0f, 1.0f);

    for (auto &p: m_frontbuffer)
    {
        // glm::vec3 pos = glm::mix(p.prev0, p.pos, alpha);
        glm::vec3 pos = p.pos;
        ren.drawSphere(pos, 0.125f);
    }

}


void
meat::ProjectileSys::createProjectile( const glm::vec3 &pos, const glm::vec3 &vel, float mass,
                                       const callback_type &callback )
{
    m_queue.push_back(Projectile(pos, vel, mass, callback));
}


