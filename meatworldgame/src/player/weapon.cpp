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




meatworld::WeaponBase::WeaponBase( const std::string &name, int parent )
:   m_parent_obj(parent)
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

    m_ignore_obj = ignore;

    int sound = AudioSystem::loadWav(desc.audio_path);
    m_emitter = AudioSystem::createEmitter(AudioSystem::Emitter(sound));
}


void
meatworld::RangedWeapon::attack( idk::EngineAPI &api )
{
    using namespace idk;

    m_position.z += 0.2f * m_desc.recoil;

    glm::vec3 origin = TransformSys::getWorldPosition(m_parent_obj);
    glm::vec3 dir = TransformSys::getFront(m_parent_obj);

    auto callback = [&api, dir]( glm::vec3 pos, glm::vec3 N )
    {
        // ParticleSystem::EmitterDesc edesc = {
        //     // .model_id  = api.getRenderer().loadModel("assets/models/effects/bullet-spark.idkvi"),
        //     .model_id  = api.getRenderer().loadModel("assets/models/effects/fire-swirl.idkvi"),
        //     .particles = 64,
        //     .duration  = 60.0f
        // };

        // ParticleSystem::ParticleDesc pdesc = {
        //     .origin_rng   = glm::vec3(0.5f),

        //     .velocity     = 8.0f * glm::reflect(dir, N),
        //     .velocity_rng = 0.5f * glm::vec4(1.0f),

        //     .scale        = glm::vec3(1.0f),
        //     .scale_factor = glm::vec3(0.0f, 1.0f, 0.0f),
        //     .scale_rng    = 0.05f,

        //     .duration     = 0.75f,
        //     .duration_rng = 0.2f,
        // };

        ParticleSystem::EmitterDesc edesc = {
            .model_id  = api.getRenderer().loadModel("assets/models/effects/bullet-spark.idkvi"),
            .particles = 8,
            .duration  = 0.08f
        };

        ParticleSystem::ParticleDesc pdesc = {
            .origin_rng   = glm::vec3(0.5f),

            .velocity     = 2.0f * glm::reflect(dir, N),
            .velocity_rng = 1.0f * glm::vec4(1.0f),

            .scale        = glm::vec3(0.1f, 0.1f, 0.25f),
            .scale_factor = glm::vec3(0.0f, 0.0f, 0.0f),
            .scale_rng    = 0.05f,

            .duration     = 0.081f,
            .duration_rng = 0.01f,
        };


        ParticleSystem::createEmitter(pos, glm::vec3(0.0f, 1.0f, 0.0f), edesc, pdesc);
    };


    bool aim = false;

    if (api.getEventSys().mouseDown(MouseButton::RIGHT))
    {
        aim = true;
    }

    WeaponSys::createProjectile(origin, dir, m_desc, aim, callback, m_ignore_obj);
    AudioSystem::playSound(m_emitter, false);
}



void
meatworld::RangedWeapon::update( idk::EngineAPI &api, float dx, float dy )
{
    using namespace idk;

    float dt = api.getEngine().deltaTime();

    glm::vec3 pos   = TransformSys::getWorldPosition(m_parent_obj);
    glm::vec3 front = TransformSys::getFront(m_parent_obj);

    AudioSystem::getEmitter(m_emitter).pos = pos + front;

    if (api.getEventSys().mouseDown(MouseButton::RIGHT))
    {
        auto &cmp = ECS2::getComponent<CameraCmp>(m_parent_obj);

        float target = m_desc.aim_fov;

        if (cmp.camera.fov_offset > target)
        {
            cmp.camera.fov_offset -= 200.0f * dt;
        }
    }

    else
    {
        auto &cmp = ECS2::getComponent<CameraCmp>(m_parent_obj);

        if (cmp.camera.fov_offset < 0.0f)
        {
            cmp.camera.fov_offset += 200.0f * dt;
        }
    }



    glm::vec3 target = m_desc.rest;
    glm::vec3 speed  = m_desc.aim_speed;

    if (api.getEventSys().mouseDown(MouseButton::RIGHT))
    {
        target = m_desc.aim;
    }


    auto &ren = api.getRenderer();

    if (m_model == -1)
    {
        m_model = ren.loadModel(m_desc.model_path);
    }

    glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);
    ren.drawModelViewspace(m_model, T);

    glm::vec3 dir = target - m_position;
    m_position += speed * dir;

    m_position.x -= m_desc.sway_speed.x * dx;
    m_position.y += m_desc.sway_speed.y * dy;
    m_position.y += m_desc.sway_speed.y * dy;

}




meatworld::NPC_RangedWeapon::NPC_RangedWeapon( int parent, const WeaponDesc &desc, int ignore )
:   WeaponBase (desc.name, parent),
    m_desc     (desc)
{
    using namespace idk;

    m_ignore_obj = ignore;
    m_weapon_obj = ECS2::createGameObject("model", false);

    ECS2::giveComponent<AudioEmitterCmp>(m_weapon_obj);
    ECS2::giveComponent<ModelCmp>(m_weapon_obj);
    ECS2::giveComponent<WeaponCmp>(m_weapon_obj);
    ECS2::getComponent<WeaponCmp>(m_weapon_obj).ignore_obj = ignore;

    WeaponSys::config(m_weapon_obj, m_desc);

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
meatworld::NPC_RangedWeapon::attack( idk::EngineAPI &api )
{
    using namespace idk;

    glm::vec3 origin, dir;

    TransformSys::translateLocalspace(m_weapon_obj, glm::vec3(0.0f, 0.0f, 0.2f * m_desc.recoil));
    
    origin = TransformSys::getWorldPosition(m_weapon_obj);
    dir    = TransformSys::getFront(m_weapon_obj);

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
    AudioSys::playSound(m_weapon_obj, false);
}


void
meatworld::NPC_RangedWeapon::update( idk::EngineAPI &api, float dx, float dy )
{
    using namespace idk;

    glm::vec3 target = m_desc.rest;
    glm::vec3 speed  = m_desc.aim_speed;

    glm::vec3 &position = TransformSys::getLocalPosition(m_weapon_obj);

    position.x -= m_desc.sway_speed.x * dx;
    position.y += m_desc.sway_speed.y * dy;

    glm::vec3 dir = target - position;
    position += speed * dir;


}




