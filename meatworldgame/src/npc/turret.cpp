#include "turret.hpp"
#include "../character/behaviour.hpp"
#include "../world/world.hpp"
#include "../player/player.hpp"
#include "../item/gyro.hpp"
#include "../item/sonar.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKIO/IDKIO.hpp>

using namespace meat;


static idk::EngineAPI *api_ptr = nullptr;


CharacterTurret::CharacterTurret( World &world, const glm::vec3 &pos, float yaw )
:   ActorBase(world, "Turret", pos),
    m_weapon(new WeaponAR2(world, this))
{
    getActorDesc().run_spd  = 0.01f;
    getActorDesc().walk_spd = 0.25f;

    idk::ECS2::giveComponent<idk::ModelCmp>(m_attach_obj);
    idk::ModelSys::assignModel(m_attach_obj, "assets/models/ship.idkvi");


    auto fn_lookTo = [this, &world]()
    {
        auto *P = world.getNearest<CharacterPlayerFPS>(getPosition(), 256.0f);

        if (P)
        {
            this->lookTo(P->getPosition());
            return BNode::SUCCESS;
        }

        return BNode::FAILURE;
    };

    auto fn_lookingAt = [this, &world]()
    {
        auto *P = world.getNearest<CharacterPlayerFPS>(getPosition(), 256.0f);

        if (P == nullptr)
        {
            return BNode::FAILURE;
        }

        glm::vec3 dir   = glm::normalize(P->getPosition() - this->getPosition());
        glm::vec3 front = idk::TransformSys::getFront(m_attach_obj);
    
        if (glm::dot(dir, front) > 0.9995f)
        {
            return BNode::SUCCESS;
        }

        return BNode::FAILURE;
    };

    auto fn_attack = [this, &world]()
    {
        m_weapon->attack(*api_ptr, world);
        return BNode::SUCCESS;
    };

    auto *node = new BNodeSequence({
        new BNodeAction(fn_lookTo),
        new BNodeAction(fn_lookingAt),
        new BNodeAction(fn_attack)
    });

    setBehaviour(node);

}



void
CharacterTurret::update( idk::EngineAPI &api, meat::World &world )
{
    api_ptr = &api;
    float dt = api.dtime();
    // auto  *P = world.getNearest<CharacterPlayerFPS>(getPosition(), 256.0f);

    // if (P)
    // {
    //     this->lookTo(P->getPosition());

    //     if (rand()%100 < 75)
    //     {
    //         m_weapon->attack(api, world);
    //     }
    // }

    m_weapon->update(api, 0.0f, 0.0f);

    ActorBase::update(api, world);
}


void CharacterTurret::lookTo( const glm::vec3 &position )
{
    glm::vec3 front = glm::normalize(position - getPosition());
    glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::cross(front, up);

    glm::quat R0   = glm::quat_cast(glm::mat3(idk::TransformSys::getModelMatrix(m_attach_obj)));
    glm::quat R1   = glm::quat_cast(glm::mat3(right, up, -front));
    glm::mat4 T    = glm::mat4_cast(glm::slerp(R0, R1, 0.025f));
              T[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    idk::TransformSys::getTransform(m_attach_obj) = idk::Transform::fromGLM(T);
}



