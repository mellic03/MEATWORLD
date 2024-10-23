#include "drone.hpp"
#include "../character/behaviour.hpp"
#include "../world/world.hpp"
#include "../player/player.hpp"
#include "../item/gyro.hpp"
#include "../item/sonar.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKIO/IDKIO.hpp>

using namespace meat;



CharacterDrone::CharacterDrone( World &world, const glm::vec3 &pos, float yaw )
:   ActorBase(world, "Drone", pos),
    m_PID(0.5f, 0.05f, 0.01f)
{
    auto fn = [this]( glm::vec3 offset )
    {
        auto *t = new ItemThruster(this, offset);
              t->enabled(true);
              t->thrust(9.85f);

        return t;
    };

    getActorDesc().run_spd  = 0.01f;
    getActorDesc().walk_spd = 0.25f;

    m_thrusters[0] = fn(glm::vec3(-1.5f, 0.25f, -1.5f));
    m_thrusters[1] = fn(glm::vec3(-1.5f, 0.25f, +1.5f));
    m_thrusters[2] = fn(glm::vec3(+1.5f, 0.25f, -1.5f));
    m_thrusters[3] = fn(glm::vec3(+1.5f, 0.25f, +1.5f));

    giveBehaviour<BehaviourGravity>();
    giveItem<meat::ItemSonar>(glm::vec3(0.0f, -1.0f, 0.0f));
    giveItem<meat::ItemGyro>();

    idk::ECS2::giveComponent<idk::ModelCmp>(m_attach_obj);
    idk::ModelSys::assignModel(m_attach_obj, "assets/models/ship.idkvi");

    BNodeComposite *node = new BNodeComposite({
        new BNodeSequence({
            new BNodeReachTargetHeight(this)
        })
    });

    setBehaviour(node);
}



void
CharacterDrone::update( idk::EngineAPI &api, meat::World &world )
{
    float dt = api.dtime();

    glm::vec3 dmove = glm::vec3(0.0f);
    glm::vec2 dlook = glm::vec2(0.0f);

    if (idkio::keyDown(idk::Keycode::UP))    dmove.x += 1.0f;
    if (idkio::keyDown(idk::Keycode::DOWN))  dmove.x -= 1.0f;
    if (idkio::keyDown(idk::Keycode::O))     dmove.y += 1.0f;
    if (idkio::keyDown(idk::Keycode::L))     dmove.y -= 1.0f;
    if (idkio::keyDown(idk::Keycode::RIGHT)) dlook.x += 0.25f;
    if (idkio::keyDown(idk::Keycode::LEFT))  dlook.x -= 0.25f;


    auto *P = world.getNearest<CharacterPlayerFPS>(getPosition(), 8.0f);

    if (P)
    {
        world.textPrompt("[E] Drone controls");

        if (idkio::keyTapped(idk::Keycode::E))
        {
            world.ui.EM.emit("menu-open", world.ui.settings);
        }
    }


    // this->move(dmove);
    this->look(dlook);


    ActorBase::update(api, world);
}


void CharacterDrone::look( const glm::vec2 &delta )
{
    float speed = getActorDesc().walk_spd;

    for (int i=0; i<4; i++)
    {
        m_thrusters[i]->tilt(0.0f, speed*delta.x);
    }
}


void CharacterDrone::move( const glm::vec3 &delta )
{
    float speed1 = getActorDesc().walk_spd;
    float speed2 = getActorDesc().run_spd;

    idk::TransformSys::pitch(m_attach_obj, delta.x);

    for (int i=0; i<4; i++)
    {
        m_thrusters[i]->tilt(speed1*delta.x, 0.0f);
        // m_thrusters[i]->thrust(delta.y);
    }
}



void CharacterDrone::moveTo( const glm::vec3 &pos )
{
    glm::vec3 dir = glm::normalize(pos - getPosition());
    addForce(4.0f*dir);
}












BNode::Status
BNodeReachTargetAngle::update( idk::EngineAPI &api, meat::World &world )
{
    auto *A = dynamic_cast<CharacterDrone*>(m_actor);

    // A->m_thrusters

    return FAILURE;
}



BNode::Status
BNodeReachTargetHeight::update( idk::EngineAPI &api, meat::World &world )
{
    float dt = api.dtime();

    auto *A = dynamic_cast<CharacterDrone*>(m_actor);
    auto *S = A->getItem<ItemSonar>();
    float reading;


    glm::vec3 force = glm::vec3(0.0f);
    for (int i=0; i<4; i++)
    {
        force += A->m_thrusters[i]->update(api);
    }
    A->addForce(force);


    if (S->read(&reading))
    {
        float error = A->m_target_y - reading;
        float mass  = A->getActorDesc().mass;
        float grav  = idk::PhysicsConstants::G;

        float thrust = mass * grav*grav * A->m_PID.iterate(error, dt);

        for (int i=0; i<4; i++)
        {
            A->m_thrusters[i]->thrust(thrust);
        }
    }


    return RUNNING;
}
