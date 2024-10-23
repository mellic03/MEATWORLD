#include "weapon.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-audio.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-lightsource.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <IDKGraphics/particle_system.hpp>

#include "../systems/sys-weapon.hpp"



meatworld::RangedWeapon::RangedWeapon( int parent, const WeaponDesc &desc, int ignore )
:   m_parent     (parent),
    m_ignore_obj (ignore),
    m_desc       (desc)
{
    using namespace idk;

    m_obj_id = ECS2::createGameObject(m_desc.name, false);

    ECS2::giveChild(parent, m_obj_id);
    TransformSys::getLocalPosition(m_obj_id) = glm::vec3(0.0f);

    std::cout << "RangedWeapon::RangedWeapon\n";
    std::cout << "    obj_id = " << m_obj_id << "\n";


    ECS2::giveComponent<AudioEmitterCmp>(m_obj_id);
    ECS2::giveComponent<ModelCmp>(m_obj_id);
    ECS2::giveComponent<WeaponCmp>(m_obj_id);
    ECS2::getComponent<WeaponCmp>(m_obj_id).ignore_obj = ignore;
    WeaponSys::config(m_obj_id, m_desc);

    AudioSys::assignSound(m_obj_id, m_desc.audio_path);
    ModelSys::assignModel(m_obj_id, m_desc.model_path);

}


float
meatworld::RangedWeapon::attack( idk::EngineAPI &api )
{
    using namespace idk;

    glm::vec3 origin, dir;

    TransformSys::translateLocalspace(m_obj_id, glm::vec3(0.0f, 0.0f, 0.2f * m_desc.recoil));
    
    origin = TransformSys::getWorldPosition(m_obj_id);
    dir    = TransformSys::getFront(m_obj_id);

    auto callback = [&api, origin, dir]( glm::vec3 pos, glm::vec3 N )
    {
        ParticleSystem::EmitterDesc edesc = {
            .model_id  = api.getRenderer().loadModel("assets/models/effects/bullet-spark.idkvi"),
            .particles = 6,
            .duration  = 0.1f
        };

        ParticleSystem::ParticleDesc pdesc = {
            .origin_rng   = glm::vec3(0.0f),

            .velocity     = 12.0f * glm::reflect(dir, N),
            .velocity_rng = 2.0f * glm::vec4(1.0f),

            .scale        = glm::vec3(0.25f),
            .scale_factor = glm::vec3(1.0f, 0.0f, 0.0f),
            .scale_rng    = 0.01f,

            .duration     = 0.11f,
            .duration_rng = 0.0f,
        };

        ParticleSystem::createEmitter(pos, glm::vec3(0.0f, 1.0f, 0.0f), edesc, pdesc);
    };

    WeaponSys::createProjectile(origin, dir, m_desc, false, callback, m_ignore_obj);
    AudioSys::playSound(m_obj_id, false);

    m_offset.z += 0.2f * m_desc.recoil;

    return 0.2f * m_desc.recoil;
}



void
meatworld::RangedWeapon::update( idk::EngineAPI &api, float dx, float dy, float *fov_offset )
{
    using namespace idk;

    glm::vec4 target;
    glm::vec3 speed;
    float     vfov;

    if (m_aiming)
    {
        target = m_desc.aim;
        speed  = m_desc.aim_speed;
        vfov   = m_desc.aim_fov;
    }

    else
    {
        target = m_desc.rest;
        speed  = m_desc.rest_speed;
        vfov   = 0.0f;
    }

    if (fov_offset)
    {
        *fov_offset = glm::mix(*fov_offset, vfov, 0.1f);
    }

    m_offset = glm::mix(m_offset, target, 0.1f);
    m_offset.x -= speed.x * dx;
    m_offset.y += speed.y * dy;

    TransformSys::getTransformCmp(m_obj_id).yaw = m_offset.w;
    TransformSys::getLocalPosition(m_obj_id) = glm::vec3(m_offset);

}




