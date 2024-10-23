#include "thruster.hpp"
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/particle/particle.hpp>

using namespace meat;



ItemThruster::ItemThruster( ActorBase *owner, const glm::vec3 &offset )
:   ItemBase(owner),
    m_obj(idk::ECS2::createGameObject("thruster", false))
{
    using namespace idk;

    ECS2::giveChild(owner->getAttachID(), m_obj);
    TransformSys::getLocalPosition(m_obj) = offset;
    TransformSys::getTransform(m_obj).rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // TransformSys::getTransformCmp(m_obj).roll  = glm::radians(45.0f);
    // TransformSys::getXYZScale(m_obj) = glm::vec3(0.25f);

    // ECS2::giveComponent<ModelCmp>(m_obj);
    // ModelSys::assignModel(m_obj, "assets/models/unit-sphere.idkvi");
}


void
ItemThruster::create_emitter( idk::EngineAPI &api, const glm::vec3 &pos, const glm::vec3 &front )
{
    idk::ParticleSystem::EmitterDesc edesc = {
        .model_id  = api.getRenderer().loadModel("assets/models/fire-sprite.idkvi"),
        .duration  = -1.0f
    };

    idk::ParticleSystem::ParticleDesc pdesc = {
        .count        = 32,
        .origin       = pos,
        .origin_rng   = glm::vec3(0.05f),
        .vel          = front,
        .vel_rng      = 2.0f * glm::vec3(1.0f),
        .scale_start  = 0.75f,
        .scale_end    = 0.25f,
        .scale_rng    = 0.1f,
        .duration     = 0.15f,
        .duration_rng = 0.1f,
        .spawn_time   = 0.01f
    };

    m_emitter = idk::ParticleSystem::createEmitter(edesc, pdesc);
}



glm::vec3
ItemThruster::update( idk::EngineAPI &api )
{
    int   obj  = m_owner->getID();
    float mass = m_owner->getActorDesc().mass;

    glm::vec3 ppos  = idk::TransformSys::getWorldPosition(obj);
    glm::vec3 wpos  = idk::TransformSys::getWorldPosition(m_obj);
    glm::vec3 lpos  = idk::TransformSys::getLocalPosition(m_obj);

    glm::mat3 R0 = glm::mat3(idk::TransformSys::getModelMatrix(obj));
    glm::mat3 R1 = glm::mat3(idk::TransformSys::getModelMatrix(m_obj));
    glm::mat3 Rx = glm::mat3(1.0f);
    
    if (glm::sign(lpos.z) == glm::sign(m_tilt.x))
    {
        Rx = glm::mat3_cast(glm::angleAxis(m_tilt.x, glm::vec3(1, 0, 0)));
    }

    glm::mat3 Ry = glm::mat3_cast(glm::angleAxis(m_tilt.y, glm::normalize(ppos-wpos)));
    glm::vec3 front = Ry * R0 * Rx * R1 * glm::vec3(0.0f, 0.0f, -1.0f);


    if (m_first)
    {
        m_first = false;
        create_emitter(api, wpos, front);
    }

    auto &e = idk::ParticleSystem::getEmitter(m_emitter);
    e.pdesc.origin = wpos + 0.5f * front;
    e.pdesc.vel    = (m_thrust/32.0f) * front;
    e.pdesc.scale_start = glm::clamp(m_thrust / 8.0f, 0.1f, 1.5f);
    e.pdesc.scale_end   = 0.25f;


    m_tilt *= 0.98f;

    if (fabs(m_thrust) < 0.0001f)
    {
        return glm::vec3(0.0f);
    }

    glm::vec3 F = m_thrust * -front;
    glm::vec3 r = (ppos - wpos);
    glm::vec3 torque = glm::cross(r, F);

    glm::vec3 mo(1.0f);
    glm::vec3 angular = api.dtime() * (torque / (mo * mass));
    glm::vec3 linear  = api.dtime() * (m_thrust / (mo * mass)) * -front;

    glm::vec3 axis  = glm::normalize(angular);
    float     angle = glm::radians(glm::length(angular));

    idk::TransformSys::rotateWorldAxis(obj, axis, angle);

    return linear;
}
