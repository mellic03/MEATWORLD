#include "sys-weapon.hpp"

#include <IDKBuiltinCS/sys-audio.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>

#include <libidk/idk_random.hpp>



int decal;


void
meatworld::WeaponSys::init( idk::EngineAPI &api )
{
    using namespace idk;

    decal = api.getRenderer().loadModel("assets/models/decal/bullet-decal.idkvi");
}



static std::vector<glm::vec3> A;
static std::vector<glm::vec3> B;
static std::deque<glm::vec3> C;
static std::deque<glm::vec3> D;

void
meatworld::WeaponSys::update( idk::EngineAPI &api )
{
    using namespace idk;

    auto &ren = api.getRenderer();


    for (int i=0; i<A.size(); i++)
    {
        ren.drawSphere(A[i], 0.1f);
    }

    for (int i=0; i<C.size(); i++)
    {
        // ren.drawSphere(C[i], 0.1f);
        ren.drawDecal(decal, C[i]+0.01f*D[i], D[i], 0.15f);
    }

    if (C.size() > 50)
    {
        C.pop_front();
        D.pop_front();
    }

    for (auto &cmp: ECS2::getComponentArray<WeaponCmp>())
    {
        if (cmp.fire)
        {
            glm::vec3 pos = TransformSys::getWorldPosition(cmp.obj_id);
            glm::vec3 dir = TransformSys::getFront(cmp.obj_id);

            float spread = cmp.config.spread;

            for (int i=0; i<cmp.config.shots; i++)
            {
                glm::vec3 offset = glm::vec3(
                    idk::randf(-spread, +spread),
                    idk::randf(-spread, +spread),
                    idk::randf(-spread, +spread)
                );

                _raycast(api, cmp, pos, glm::normalize(dir + offset));
            }

            cmp.fire = false;

            ECS2::getComponent<PointlightCmp>(cmp.obj_id).light.diffuse = glm::vec4(0.0f);
        }


    }


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
meatworld::WeaponSys::config( int obj_id, const WeaponConfig &config )
{
    idk::ECS2::getComponent<WeaponCmp>(obj_id).config = config;
}


void
meatworld::WeaponSys::fire( int obj_id )
{
    idk::ECS2::getComponent<WeaponCmp>(obj_id).fire = true;
    idk::ECS2::getComponent<idk::PointlightCmp>(obj_id).light.diffuse = glm::vec4(2.6f, 2.6f, 1.7f, 1.0f);
}


void
meatworld::WeaponSys::_raycast( idk::EngineAPI &api, meatworld::WeaponCmp &wcmp, const glm::vec3 &origin,
                                const glm::vec3 &dir )
{
    using namespace idk;

    glm::vec3 res;
    glm::vec3 N;

    for (auto &cmp: ECS2::getComponentArray<HitSphereCmp>())
    {
        glm::vec3 sphere_pos = TransformSys::getWorldPosition(cmp.obj_id);
        float radius = 0.5f * TransformSys::getXYZScale(cmp.obj_id).x;

        if (geometry::raySphereIntersects(origin, dir, sphere_pos, radius, &res))
        {
            C.push_back(res);
            D.push_back(N);
            cmp.is_hit = true;
            cmp.hit    = res;
        }
    }

    for (auto &cmp: ECS2::getComponentArray<HitBoxCmp>())
    {
        glm::mat4 M = TransformSys::getModelMatrix(cmp.obj_id);
        glm::vec3 bounds = glm::mat3(M) * glm::vec3(0.5f, 0.5f, 0.5f);

        glm::vec3 rect_pos = TransformSys::getWorldPosition(cmp.obj_id);

        if (geometry::rayAABBIntersection(origin, dir, rect_pos, bounds, &res, &N))
        {
            cmp.is_hit = true;
            cmp.hit    = res;

            C.push_back(res);
            D.push_back(N);
        }
    }

    for (auto &cmp: ECS2::getComponentArray<CharacterHitBoxCmp>())
    {
        glm::mat4 M = TransformSys::getModelMatrix(cmp.obj_id);
        glm::vec3 bounds = glm::mat3(M) * glm::vec3(0.5f, 0.5f, 0.5f);

        glm::vec3 rect_pos = TransformSys::getWorldPosition(cmp.obj_id);

        if (cmp.obj_id == wcmp.ignore_obj)
        {
            continue;
        }

        if (geometry::rayAABBIntersection(origin, dir, rect_pos, bounds, &res, &N))
        {
            cmp.is_hit = true;
            cmp.hit    = res;

            cmp.callback();
        }
    }
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


