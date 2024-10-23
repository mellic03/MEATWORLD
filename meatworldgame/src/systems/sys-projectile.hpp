#pragma once

#include <IDKECS/IDKECS.hpp>
#include <vector>
#include <array>


namespace idk
{
    class EngineAPI;
}

namespace meat
{
    struct Projectile;
    class  ProjectileSys;
}



struct meat::Projectile
{
    glm::vec3 prev0, prev1;
    glm::vec3 pos, vel, acc;
    float     mass;

    using callback_type = std::function<void( idk::EngineAPI&, const glm::vec3&, const glm::vec3& )>;
    callback_type callback;

    Projectile()
    {
        
    }

    Projectile( const glm::vec3 &p, const glm::vec3 &v, float m, const callback_type &fn )
    :   pos(p), vel(v), acc(0.0f),
        mass(m),
        prev0(p), prev1(p),
        callback(fn)
    {

    }
};


class meat::ProjectileSys: public idk::ECS2::System
{
private:
    using callback_type = std::function<void( idk::EngineAPI&, const glm::vec3&, const glm::vec3& )>;

    inline static std::vector<Projectile> m_frontbuffer;
    inline static std::vector<Projectile> m_backbuffer;
    inline static std::vector<Projectile> m_queue;
    inline static std::vector<int>        m_cull;

    static void _update( float dt );
    static void _cull( idk::EngineAPI& );


public:
    virtual void init   ( idk::EngineAPI& ) final;
    virtual void update ( idk::EngineAPI& ) final;

    static void createProjectile( const glm::vec3 &pos, const glm::vec3 &vel, float mass,
                                  const callback_type &callback = [](idk::EngineAPI&, const glm::vec3&, const glm::vec3&){} );

};

