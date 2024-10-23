#pragma once

#include "../character/base.hpp"
#include "../character/character.hpp"
// #include "../behaviour/behaviour.hpp"


namespace meat
{
    // class BehaviourHumanoidMovement;
    // class BehaviourBossShit;
    class BNodeActionEnemyBossShit;

    class CharacterHumanoid;
}



// class meat::BehaviourHumanoidMovement: public BehaviourBase
// {
// private:
//     float m_timer = 5.0f;
//     glm::vec3 m_dest = glm::vec3(0.0f);

// public:
//     virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
// };


// class meat::BehaviourBossShit: public BehaviourBase
// {
// private:
//     meat::BNode *m_btree;
//     int m_emitter;

// public:
//     BehaviourBossShit( ActorBase *A );
//     ~BehaviourBossShit();

//     virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
// };



// class meat::BNodeActionEnemyBossShit: public BNodeAction
// {
// private:

// public:
//     using BNodeAction::BNodeAction;
//     virtual Status update( idk::EngineAPI&, meat::World&) final;
// };



// class meat::CharacterHumanoid: public ActorBase
// {
// private:

// public:
//     CharacterHumanoid( World &world, const glm::vec3 &pos=glm::vec3(0.0f), float yaw=0.0f );

//     virtual void update( idk::EngineAPI&, meat::World& ) override;

//     virtual void look( const glm::vec2& );
//     virtual void move( idk::EngineAPI&, const glm::vec3& );
//     virtual void moveTo( const glm::vec3& );
//     virtual void jump();
// };



