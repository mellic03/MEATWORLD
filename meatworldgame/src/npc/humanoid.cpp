#include "humanoid.hpp"
#include "../world/world.hpp"
#include "../character/player.hpp"

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

using namespace meat;



// void
// BehaviourHumanoidMovement::update( idk::EngineAPI &api, meat::World &world, ActorBase *A )
// {
//     float dt = api.dtime();

//     m_timer -= dt;

//     if (m_timer <= 0.0f)
//     {
//         m_timer = 5.0f;

//         auto *P = world.getNearest<CharacterPlayerFPS>(A->getPosition(), 100.0f);

//         if (P)
//         {
//             m_dest = P->getPosition();
//         }
//     }

//     else
//     {
//         A->moveTo(m_dest);
//     }

// }


// BehaviourBossShit::BehaviourBossShit( ActorBase *A )
// :   BehaviourBase(A)
// {
//     m_emitter = idk::ECS2::createGameObject("emitter", false);
//     idk::ECS2::giveComponent<idk::AudioEmitterCmp>(m_emitter);
//     idk::AudioSys::assignSound(m_emitter, "assets/audio/music/midra2.wav");

//     m_btree = new BNodeSequence({
//         new BNodeActionEnemyBossShit(A)
//     });
// }


// BehaviourBossShit::~BehaviourBossShit()
// {
//     idk::ECS2::deleteGameObject(m_emitter);
// }


// void
// BehaviourBossShit::update( idk::EngineAPI &api, meat::World &world, ActorBase *A )
// {
//     BNode::Status status = m_btree->update(api, world);

//     if (status == BNode::Status::SUCCESS)
//     {
//         if (idk::AudioSys::isPlaying(m_emitter) == false)
//         {
//             world.renderText(2, 2, "[E] Play audio");

//             if (api.getEventSys().keylog().keyTapped(idk::Keycode::E))
//             {
//                 idk::AudioSys::playSound(m_emitter, false);
//             }
//         }
//     }
// }


// BNode::Status
// BNodeActionEnemyBossShit::update( idk::EngineAPI &api, meat::World &world )
// {
//     ActorBase *A = m_actor;

//     auto *P = world.getNearest<CharacterPlayerFPS>(A->getPosition(), 16.0f);

//     if (P == nullptr)
//     {
//         return Status::FAILURE;
//     }

//     glm::vec3 dir = glm::normalize(A->getPosition() - P->getPosition());

//     if (glm::dot(dir, P->getFront()) < 0.5f)
//     {
//         return Status::FAILURE;
//     }

//     return Status::SUCCESS;
// }



// CharacterHumanoid::CharacterHumanoid( World &world, const glm::vec3 &pos, float yaw )
// :   ActorBase(world, "Humanoid", pos, yaw)
// {
//     // giveBehaviour<BehaviourGravity>();
//     // giveBehaviour<BehaviourCollideWithTerrain>();
//     // giveBehaviour<BehaviourBossShit>();

//     idk::ECS2::giveComponent<idk::ModelCmp>(m_attach_obj);
//     idk::ModelSys::assignModel(m_attach_obj, "assets/models/npc/meat-torso.idkvi");

// }



// void
// CharacterHumanoid::look( const glm::vec2 &pos )
// {

// }


// void
// CharacterHumanoid::move( idk::EngineAPI &api, const glm::vec3 &delta )
// {

// }


// void
// CharacterHumanoid::moveTo( const glm::vec3 &pos )
// {
//     glm::vec3 &char_pos = idk::TransformSys::getLocalPosition(m_root_obj);
//     glm::vec3  dir      = glm::normalize(pos - char_pos);

//     char_pos += 0.1f * dir;
// }


// void
// CharacterHumanoid::jump()
// {

// }



