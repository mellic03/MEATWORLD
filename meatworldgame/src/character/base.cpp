#include "base.hpp"
#include "../world/world.hpp"
#include "../systems/sys-actor.hpp"

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

using namespace meat;



ItemBase::ItemBase( ActorBase *owner ): m_owner(owner) {  }
ItemBase::~ItemBase() {  }


ItemWrapper::ItemWrapper( World &world, const glm::vec3 &pos, ItemBase *item )
:   ActorBase(world, "ItemWrapper", pos, 0.0f),
    // iInteractive([](idk::EngineAPI&, meat::World&) { std::cout << "ItemWrapper::ItemWrapper callback!!!\n"; }),
    m_item(item)
{
    m_item->setOwner(this);
}


void
ItemWrapper::update( idk::EngineAPI &api, meat::World &world )
{
    auto &ren = api.getRenderer();

    if (m_model == -1)
    {
        m_model = ren.loadModel(m_item->getModelPath());
    }

    this->setAngular(glm::vec4(0.0f, 0.0f, 0.0f, 0.025f));
    ren.drawModel(m_model, this->getTransform(), idk::TransformSys::getTransformCmp(m_root_obj).prev_model);

    ActorBase::update(api, world);
}



ActorBase::ActorBase( World &w, const std::string &name, const glm::vec3 &pos, float yaw )
:   world(w), m_name(name), m_btree(nullptr)
{
    m_root_obj   = idk::ECS2::createGameObject(name, false);
    m_attach_obj = idk::ECS2::createGameObject("attachment", false);

    idk::ECS2::giveComponent<meat::ActorCmp>(m_root_obj);
    idk::ECS2::getComponent<meat::ActorCmp>(m_root_obj).actor = this;

    idk::ECS2::giveComponent<idk::TransformCmp>(m_root_obj);
    idk::ECS2::giveComponent<idk::TransformCmp>(m_attach_obj);
    idk::ECS2::giveChild(m_root_obj, m_attach_obj);
    idk::TransformSys::getLocalPosition(m_attach_obj) = glm::vec3(0.0f);

    auto &cmp = idk::TransformSys::getTransformCmp(m_root_obj);
    cmp.transform.position = pos;
    cmp.yaw = yaw;
}


ActorBase::~ActorBase()
{
    idk::ECS2::deleteGameObject(m_attach_obj);
    idk::ECS2::deleteGameObject(m_root_obj);

    // for (auto &[key, B]: m_behaviours)
    // {
    //     delete B;
    // }

    for (auto &[key, item]: m_items)
    {
        delete item;
    }

    // m_behaviours.clear();
    m_items.clear();

}


glm::mat4
ActorBase::getTransform()
{
    return idk::TransformSys::getModelMatrix(m_root_obj);

    // glm::mat4 M    = idk::TransformSys::getModelMatrix(m_root_obj);
    //           M[3] = glm::vec4(m_curr_world, 1.0f);
    // return M;
}


void
ActorBase::setTransform( const glm::mat4 &T )
{
    idk::TransformSys::getTransform(m_root_obj) = idk::Transform::fromGLM(T);
}


void
ActorBase::setRotation( const glm::mat3 &R )
{
    glm::vec3 pos = idk::TransformSys::getLocalPosition(m_root_obj);

    glm::mat4 T    = glm::mat4(R);
              T[3] = glm::vec4(pos, 1.0f);

    setTransform(T);
}


glm::vec3
ActorBase::getFront()
{
    return idk::TransformSys::getFront(m_root_obj);
}


glm::vec3
ActorBase::getPosition()
{
    // return m_curr_world;
    return idk::TransformSys::getWorldPosition(m_root_obj);
}


glm::vec3&
ActorBase::getLocalPosition()
{
    // return m_curr_local;
    return idk::TransformSys::getLocalPosition(m_root_obj);
}

void
ActorBase::setLocalPosition( const glm::vec3 &pos )
{
    getLocalPosition() = pos;
}


void
ActorBase::translateWorld( const glm::vec3 &v )
{
    idk::TransformSys::translateWorldspace(m_root_obj, v);
}

void
ActorBase::translateLocal( const glm::vec3 &v )
{
    idk::TransformSys::translateLocalspace(m_root_obj, v);
}



void
ActorBase::update( idk::EngineAPI &api, meat::World &world )
{
    static constexpr float scale        = 1.0f; // 0.25f;
    static constexpr float timestep     = 1.0f / 60.0f;
    static constexpr float max_timestep = 0.25f;

    float dt = api.dtime();
    // m_accum += dt;


    glm::mat4  W   = idk::TransformSys::getWorldMatrix(m_root_obj);
    glm::mat4  iM  = glm::inverse(W);
    glm::mat3  W3  = glm::mat3(W);
    glm::mat3  iW3 = glm::inverse(W3);
    glm::vec3 &pos = idk::TransformSys::getLocalPosition(m_root_obj);
    float     &yaw = idk::TransformSys::getTransformCmp(m_root_obj).yaw;

    // while (m_accum >= timestep)
    {
        // m_prev_local[1] = m_prev_local[0];

        float t = scale*dt;
        float a = glm::clamp(dt/0.02f, 0.0f, 1.0f);

        yaw += m_ang.w;
        m_ang *= m_desc.vel_damp;


        glm::vec3 accc = glm::clamp(m_acc, glm::vec3(-m_desc.acc_max), glm::vec3(+m_desc.acc_max));
                //   accc.y -= idk::PhysicsConstants::G;

        m_vel = glm::clamp(m_vel, glm::vec3(-m_desc.vel_max), glm::vec3(+m_desc.vel_max));

        // glm::vec3 imp = (iW3 * m_imp);
        glm::vec3 acc = (iW3 * accc);
        glm::vec3 vel = (iW3 * m_vel);

        {
            pos += t*vel + 0.5f * t*t*acc;
            vel += t*acc; // + t*imp;

            // m_imp = glm::mix(W3*imp, glm::vec3(0.0f), a*(1.0-m_desc.imp_damp));
            // m_acc = glm::mix(W3*acc, glm::vec3(0.0f), a*(1.0-m_desc.acc_damp));
            m_vel = glm::mix(W3*vel, glm::vec3(0.0f), a*(1.0-m_desc.vel_damp));

            // m_accum -= timestep;
        }

        m_acc = glm::vec3(0.0f);
        // m_prev_local[0] = pos;
    }

    // m_alpha      = glm::clamp(m_accum/timestep, 0.0f, 1.0f);
    // m_curr_local = glm::mix(m_prev_local[1], m_prev_local[0], m_alpha);
    // m_curr_world = glm::vec3(W * glm::vec4(m_curr_local, 1.0f));

    // idk::TransformSys::getLocalPosition(m_root_obj) = m_curr_local;

    if (m_btree)
    {
        m_btree->updateNode(api, world);
    }

    for (auto &[key, B]: m_behaviours)
    {
        if (B->enabled())
        {
            B->update(api, world, this);
        }
    }

    for (auto &[key, item]: m_items)
    {
        item->update(api, world);
    }
}



void
ActorBase::move( const glm::vec3 &delta )
{
    // glm::mat3 R = glm::inverse(glm::mat3(idk::TransformSys::getWorldMatrix(getID())));
    idk::TransformSys::translateLocalspace(getID(), delta);
}


void
ActorBase::walk( const glm::vec3 &delta )
{
    this->move(getActorDesc().walk_spd * delta);
}


void
ActorBase::run( const glm::vec3 &delta )
{
    this->move(getActorDesc().run_spd * delta);
}




