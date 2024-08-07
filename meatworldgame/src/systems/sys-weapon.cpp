#include "sys-weapon.hpp"

#include <IDKBuiltinCS/sys-audio.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>

#include <IDKGraphics/particle_system.hpp>

#include <libidk/idk_random.hpp>





namespace
{
    std::vector<meatworld::Projectile> m_projectiles;
};



static idk::EngineAPI *api_ptr;

int decal;


void
meatworld::WeaponSys::init( idk::EngineAPI &api )
{
    using namespace idk;

    api_ptr = &api;
    decal = api.getRenderer().loadModel("assets/models/decal/bullet-decal.idkvi");
}



static std::deque<glm::vec3> A;
static std::deque<glm::vec3> B;
static std::deque<glm::vec3> C;
static std::deque<glm::vec3> D;

void
meatworld::WeaponSys::update( idk::EngineAPI &api )
{
    using namespace idk;

    auto &ren = api.getRenderer();


    for (int i=0; i<A.size(); i++)
    {
        // ren.drawLine(A[i], B[i], 0.05f);
        // ren.drawSphere(A[i], 0.1f);
    }

    for (int i=0; i<C.size(); i++)
    {
        // ren.drawSphere(C[i], 0.1f);
        ren.drawDecal(decal, C[i]+0.01f*D[i], D[i], 0.15f);
    }


    if (A.size() > 50)
    {
        A.pop_front();
        B.pop_front();
    }

    if (C.size() > 50)
    {
        C.pop_front();
        D.pop_front();
    }


    for (auto &proj: m_projectiles)
    {
        glm::vec3 hit_pos;
        glm::vec3 hit_normal;
        int       hit_obj;

        glm::vec3 pos = proj.pos;
        glm::vec3 dir = proj.dir;
        auto &desc = proj.desc;

        for (int i=0; i<desc.shots; i++)
        {
            float spread = (proj.aim) ? desc.inner_spread : desc.outer_spread;

            glm::vec3 N = glm::vec3(
                idk::randf_guassian(spread, 2),
                idk::randf_guassian(spread, 2),
                idk::randf_guassian(spread, 2)
            );

            N = glm::normalize(dir + N); 

            A.push_back(pos);
            B.push_back(pos + 32.0f*N);

            if (_raycast(api, proj, pos, N, &hit_pos, &hit_normal, &hit_obj))
            {
                proj.callback(hit_pos, hit_normal);

                C.push_back(hit_pos);
                D.push_back(hit_normal);
            }
        }

    }

    m_projectiles.clear();


    // for (auto &cmp: ECS2::getComponentArray<WeaponCmp>())
    // {
    //     if (cmp.fire)
    //     {
    //         glm::vec3 pos = TransformSys::getWorldPosition(cmp.obj_id);
    //         glm::vec3 dir = TransformSys::getFront(cmp.obj_id);
    //         // glm::mat3 M   = glm::mat3(TransformSys::getModelMatrix(cmp.obj_id));

    //         for (int i=0; i<cmp.desc.shots; i++)
    //         {
    //             float spread = cmp.desc.outer_spread;

    //             if (idk::randf() < cmp.desc.inner_prob)
    //             {
    //                 spread = cmp.desc.inner_spread;
    //             }

    //             glm::vec3 N = glm::vec3(
    //                 idk::randf_guassian(spread, 2),
    //                 idk::randf_guassian(spread, 2),
    //                 -1.0f
    //             );

    //             std::cout << N.x << ", " << N.y << "\n";

    //             _raycast(api, cmp, pos, glm::normalize(dir * N));
    //         }

    //         cmp.fire = false;

    //         ECS2::getComponent<PointlightCmp>(cmp.obj_id).light.diffuse = glm::vec4(0.0f);
    //     }
    // }


    for (auto &cmp: ECS2::getComponentArray<HitSphereCmp>())
    {
        glm::vec3 pos = TransformSys::getWorldPosition(cmp.obj_id);

        if (cmp.is_hit)
        {
            cmp.is_hit = false;
        }
    }


    for (auto &cmp: ECS2::getComponentArray<HitBoxCmp>())
    {
        glm::vec3 pos = TransformSys::getWorldPosition(cmp.obj_id);

        if (cmp.is_hit)
        {
            cmp.is_hit = false;
        }
    }

}

void
meatworld::WeaponSys::config( int obj_id, const WeaponDesc &desc )
{
    idk::ECS2::getComponent<WeaponCmp>(obj_id).desc = desc;
}


void
meatworld::WeaponSys::fire( int obj_id )
{
    idk::ECS2::getComponent<WeaponCmp>(obj_id).fire = true;
    idk::ECS2::getComponent<idk::PointlightCmp>(obj_id).light.diffuse = glm::vec4(2.6f, 2.6f, 1.7f, 1.0f);
}


void
meatworld::WeaponSys::createProjectile( const glm::vec3 &origin, const glm::vec3 &dir,
                                        const WeaponDesc &desc, bool aim,
                                        std::function<void(glm::vec3, glm::vec3)> callback,
                                        int ignore )
{
    using namespace idk;
    m_projectiles.push_back({true, aim, origin, dir, desc, callback, ignore});
}


bool
meatworld::WeaponSys::_raycast( idk::EngineAPI &api, const Projectile &proj,
                                const glm::vec3 &origin, const glm::vec3 &dir,
                                glm::vec3 *hit, glm::vec3 *N, int *obj_id )
{
    using namespace idk;

    glm::vec3 nearest_hit = glm::vec3(+INFINITY);
    glm::vec3 nearest_N   = glm::vec3(+INFINITY);
    float nearest_distSq  = +INFINITY;
    int   nearest_obj     = -1;
    bool  ret_value       = false;

    glm::vec3 res;

    for (auto &cmp: ECS2::getComponentArray<HitSphereCmp>())
    {
        glm::vec3 sphere_pos = TransformSys::getWorldPosition(cmp.obj_id);
        float radius = 0.5f * TransformSys::getXYZScale(cmp.obj_id).x;

        if (geometry::raySphereIntersects(origin, dir, sphere_pos, radius, &res))
        {
            float distSq = glm::distance2(origin, res);

            if (distSq < nearest_distSq)
            {
                nearest_distSq = distSq;
                nearest_hit    = res;
                nearest_N      = *N;
                nearest_obj    = cmp.obj_id;
            }
            ret_value = true;
        }
    }

    for (auto &cmp: ECS2::getComponentArray<HitBoxCmp>())
    {
        glm::mat4 M = TransformSys::getModelMatrix(cmp.obj_id);
        glm::vec3 bounds = glm::mat3(M) * glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 rect_pos = TransformSys::getWorldPosition(cmp.obj_id);

        if (geometry::rayAABBIntersection(origin, dir, rect_pos, bounds, &res, N))
        {
            float distSq = glm::distance2(origin, res);

            if (distSq < nearest_distSq)
            {
                nearest_distSq = distSq;
                nearest_hit    = res;
                nearest_N      = *N;
                nearest_obj    = cmp.obj_id;
            }
            ret_value = true;
        }
    }

    for (auto &cmp: ECS2::getComponentArray<CharacterHitBoxCmp>())
    {
        glm::mat4 M = TransformSys::getModelMatrix(cmp.obj_id);
        glm::vec3 bounds = glm::mat3(M) * glm::vec3(0.5f, 0.5f, 0.5f);

        glm::vec3 rect_pos = TransformSys::getWorldPosition(cmp.obj_id);

        if (cmp.obj_id == proj.ignore)
        {
            continue;
        }

        if (geometry::rayAABBIntersection(origin, dir, rect_pos, bounds, &res, N))
        {
            float distSq = glm::distance2(origin, res);

            if (distSq < nearest_distSq)
            {
                nearest_distSq = distSq;
                nearest_hit    = res;
                nearest_N      = *N;
                nearest_obj    = cmp.obj_id;
            }
            ret_value = true;
        }
    }

    {
        *hit    = nearest_hit;
        *N      = nearest_N;
        *obj_id = nearest_obj;
    }

    return ret_value;
}




size_t
meatworld::WeaponCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
meatworld::WeaponCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
meatworld::WeaponCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    idk::ECS2::giveComponent<idk::PointlightCmp>(obj_id);

    auto &cmp = idk::ECS2::getComponent<idk::PointlightCmp>(obj_id);
    cmp.light.diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    cmp.light.radius  = 2.0f;

}


void
meatworld::WeaponCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::WeaponCmp::onObjectCopy( int src_obj, int dst_obj )
{

}











size_t
meatworld::HitSphereCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
meatworld::HitSphereCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
meatworld::HitSphereCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::HitSphereCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::HitSphereCmp::onObjectCopy( int src_obj, int dst_obj )
{

}









size_t
meatworld::HitBoxCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
meatworld::HitBoxCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
meatworld::HitBoxCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::HitBoxCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::HitBoxCmp::onObjectCopy( int src_obj, int dst_obj )
{

}






size_t
meatworld::CharacterHitBoxCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
meatworld::CharacterHitBoxCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
meatworld::CharacterHitBoxCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::CharacterHitBoxCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

}


void
meatworld::CharacterHitBoxCmp::onObjectCopy( int src_obj, int dst_obj )
{

}


