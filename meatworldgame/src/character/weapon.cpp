#include "weapon.hpp"
#include "../world/world.hpp"
#include "../systems/sys-actor.hpp"
#include "../systems/sys-projectile.hpp"

#include <libidk/idk_random.hpp>
#include <IDKGraphics/particle/particle_system.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKIO/IDKIO.hpp>


using namespace meat;




WeaponGlock::WeaponGlock( meat::World &world, ActorBase *owner )
:   WeaponRanged(world, owner)
{
    idk::AudioSys::assignSound(m_obj, "assets/audio/pistol.wav");
    idk::ModelSys::assignModel(m_obj, "assets/models/weapons/glock.idkvi");
}


WeaponAR2::WeaponAR2( meat::World &world, ActorBase *owner )
:   WeaponRanged(world, owner)
{
    idk::AudioSys::assignSound(m_obj, "assets/audio/pistol.wav");
    idk::ModelSys::assignModel(m_obj, "assets/models/weapons/hl2-ar2.idkvi");
}

WeaponFlamethrower::WeaponFlamethrower( meat::World &world, ActorBase *owner )
:   WeaponAR2(world, owner)
{
    idk::AudioSys::assignSound(m_obj, "assets/audio/pistol.wav");
    idk::ModelSys::assignModel(m_obj, "assets/models/weapons/hl2-ar2.idkvi");
}


glm::vec2
WeaponFlamethrower::attack( idk::EngineAPI &api, meat::World &world )
{
    glm::vec3 wpos = idk::TransformSys::getWorldPosition(m_obj);
    glm::vec3 wdir = idk::TransformSys::getFront(m_obj);

    idk::ParticleSystem::EmitterDesc edesc = {
        .model_id  = api.getRenderer().loadModel("assets/models/fire-sprite.idkvi"),
        .duration  = -1.0f
    };

    idk::ParticleSystem::ParticleDesc pdesc = {
        .count        = 512,
        .origin       = wpos,
        .origin_rng   = glm::vec3(0.0f),
        .vel          = 2.0f * wdir,
        .vel_rng      = 1.5f * glm::vec3(1.0f),
        .scale_start  = 0.2f,
        .scale_end    = 1.0f,
        .scale_rng    = 0.1f,
        .duration     = 16.0f,
        .duration_rng = 16.0f
    };

    if (m_emitter == -1)
    {
        m_emitter = idk::ParticleSystem::createEmitter(edesc, pdesc);
    }

    return WeaponAR2::attack(api, world);
}



void
WeaponFlamethrower::update( idk::EngineAPI &api, float dx, float dy )
{
    float dt = api.dtime();

    if (m_emitter != -1)
    {
        auto &e = idk::ParticleSystem::getEmitter(m_emitter);

        glm::vec3 front = idk::TransformSys::getFront(m_obj);

        e.pdesc.origin = idk::TransformSys::getWorldPosition(m_obj) + front;
        e.pdesc.vel    = 2.0f * front;
    }

    WeaponRanged::update(api, dx, dy);
}





WeaponBase::WeaponBase( ActorBase *owner, const std::string &name )
:   ItemBase (owner),
    m_parent (owner->getAttachID()),
    m_obj    (idk::ECS2::createGameObject(name, false))
{
    idk::ECS2::giveChild(m_parent, m_obj);
    idk::TransformSys::getLocalPosition(m_obj) = glm::vec3(0.0f);

    idk::ECS2::giveComponent<idk::AudioEmitterCmp>(m_obj);
    idk::AudioSys::assignSound(m_obj, "assets/audio/pistol.wav");
}


WeaponBase::~WeaponBase()
{
    idk::ECS2::deleteGameObject(m_obj);
}






WeaponRanged::WeaponRanged( meat::World &world, ActorBase *owner )
:   WeaponBase(owner, "WeaponRanged"),
    worldref(world),
    m_rest(0.15f, -0.05f, -0.2f)
{
    idk::ECS2::giveComponent<meat::WeaponRangedCmp>(m_obj);
    idk::ECS2::getComponent<meat::WeaponRangedCmp>(m_obj).weapon = this;

    idk::ModelSys::assignModel(m_obj, "assets/models/weapons/hl2-ar2.idkvi");

    owner->world.EM.emit("WeaponRanged::WeaponRanged", owner);
}


void
WeaponRanged::update( idk::EngineAPI &api, float dx, float dy )
{
    float dt = api.dtime();

    if (m_timer > 0.0f)
    {
        m_timer -= dt;
    }


    glm::vec3 &pos = idk::TransformSys::getLocalPosition(m_obj);
    glm::vec3 md = glm::vec3(dx, -dy, 0.0f);

    glm::vec3 prev = idk::TransformSys::getTransformCmp(m_parent).prev_world[3];
    glm::vec3 curr = idk::TransformSys::getTransformCmp(m_parent).world[3];
    glm::vec3 wd = curr - prev;

    static float accum = 0.0f;
    float potential = 0.0f;
    float dist = glm::length2(wd);
    float bob = potential * 0.5f * sin(2.0f * accum);

    glm::vec3 d0 = -0.0005f * md;
    glm::vec3 d1 = glm::vec3(0.0f, bob, 0.0f);
    glm::vec3 d2 = m_rest - pos;


    float alpha = glm::clamp(10.0f*dt, 0.0f, 0.05f);

    pos += alpha * (d0 + d1);
    pos += alpha * (d2);

}


void
WeaponRanged::projectile_callback( idk::EngineAPI &api, const glm::vec3 &pos, const glm::vec3 &N )
{
    idk::ParticleSystem::EmitterDesc edesc = {
        .model_id  = api.getRenderer().loadModel("assets/models/fire-sprite.idkvi"),
        .duration  = 0.5f
    };

    idk::ParticleSystem::ParticleDesc pdesc = {
        .count        = 32,
        .origin       = pos,
        .origin_rng   = glm::vec3(0.0f),
        .vel          = 16.0f * N,
        .vel_rng      = 16.0f * glm::vec3(1.0f),
        .scale_rng    = 0.1f,
        .duration     = 0.5f,
        .duration_rng = 0.0f
    };

    idk::ParticleSystem::createEmitter(edesc, pdesc);
}




glm::vec2
WeaponRanged::attack( idk::EngineAPI &api, meat::World &world )
{
    if (m_timer > 0.0f)
    {
        return glm::vec2(0.0f);
    }

    else
    {
        m_timer = getRangedDesc().cooldown;
    }


    idk::AudioSys::playSound(m_obj, false);

    glm::vec3 &lpos = idk::TransformSys::getLocalPosition(m_obj);
    lpos.z += 0.05f;


    glm::vec3 wpos = idk::TransformSys::getWorldPosition(m_obj);
    glm::vec3 wdir = idk::TransformSys::getFront(m_obj);

    auto  &desc = getRangedDesc();
    float  vel  = desc.vel + (desc.vel_rng * idk::randf(-1.0f, +1.0f)); // (desc.vel + idk::randf(-desc.vel_rng, +desc.vel_rng)) * idk::randvec3(-1, +1);
    float  mass = desc.mass; // + idk::randf(-desc.mass_rng, +desc.mass_rng);

    meat::ProjectileSys::createProjectile(
        wpos+0.25f*wdir, vel*wdir, mass,
        [this]( idk::EngineAPI &api, const glm::vec3 &pos, const glm::vec3 &N )
        {
            worldref.hitMarker(pos);
            projectile_callback(api, pos, N);
        }
    );

    return 0.015f * idk::randvec2(-1.0f, +1.0f);
}





ItemWeaponInventory::ItemWeaponInventory( ActorBase *owner )
:   ItemBase(owner),
    m_weapon(nullptr),
    m_weapons(10, nullptr)
{

    owner->world.EM.on("VehicleBase::drive", [this]( ActorBase *A ) {
        if (A == getOwner()) { lowerWeapon(); }
    });

    owner->world.EM.on("VehicleBase::undrive", [this]( ActorBase *A ) {
        if (A == getOwner()) { raiseWeapon(); }
    });
}



static WeaponBase*
makeWeapon( meat::World &world, ActorBase *owner, int idx )
{
    switch (idx)
    {
        default: return nullptr;
        case 0: return new WeaponGlock(world, owner);
        case 1: return new WeaponAR2(world, owner);
        case 2: return new WeaponFlamethrower(world, owner);
    }
}


void
ItemWeaponInventory::update( idk::EngineAPI &api, meat::World &world )
{
    if (idkio::mouseCaptured() == false)
    {
        return;
    }

    if (m_weapon)
    {
        idk::ECS2::getComponent<idk::ModelCmp>(m_weapon->getWeaponID()).visible = !m_lowered;

        glm::vec2 md = idkio::mouseDelta();
        m_weapon->update(api, md.x, md.y);

        if (idkio::mouseClicked(idkio::LMOUSE))
        {
            world.EM.emit("player-attack", nullptr);
            glm::vec2 recoil = m_weapon->attack(api, world);
            getOwner()->look(recoil);
        }
    }

    for (int i=30; i<39; i++)
    {
        if (idkio::keyTapped(idk::Keycode(i)))
        {
            int idx = i-30;

            if (m_weapons[idx])
            {
                delete m_weapons[idx];
                m_weapons[idx] = nullptr;
            }

            else
            {
                m_weapon = nullptr;
            }

            m_weapons[idx] = makeWeapon(world, getOwner(), idx);
            m_weapon = m_weapons[idx];
        }
    }


}

// void
// BehaviourWeaponHolder::update( idk::EngineAPI &api, meat::World &world, ActorBase *C )
// {
//     auto &events = api.getEventSys();

//     if (events.mouseCaptured() == false)
//     {
//         return;
//     }


//     if (idk::IO::keyTapped(idk::Keycode::N0))
//     {
//         if (m_weapon)
//         {
//             delete m_weapon;
//             m_weapon = nullptr;
//         }
//     }

//     else if (idk::IO::keyTapped(idk::Keycode::N1))
//     {
//         if (m_weapon)
//         {
//             delete m_weapon;
//         }

//         m_weapon = new WeaponGlock(C);
//     }

//     else if (idk::IO::keyTapped(idk::Keycode::N2))
//     {
//         if (m_weapon)
//         {
//             delete m_weapon;
//         }

//         m_weapon = new WeaponAR2(C);
//     }

    


//     if (m_weapon && events.mouseCaptured())
//     {
//         glm::vec2 md = events.mouseDelta();
//         m_weapon->update(api, md.x, md.y);

//         if (events.mouseClicked(idk::MouseButton::LEFT))
//         {
//             glm::vec2 recoil = m_weapon->attack(api, world);
//             C->look(recoil);
//         }
//     }

//     // glm::vec3 &pos = idk::TransformSys::getLocalPosition(C->getID());
//     // pos.y = 1.75f + idk::TerrainRenderer::heightQuery(pos.x, pos.z);
// }



