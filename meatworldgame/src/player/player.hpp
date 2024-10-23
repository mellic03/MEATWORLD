#pragma once

#include "../character/base.hpp"


namespace meat
{
    class  BehaviourUserKeyInput;
    class  BehaviourUserMouseInput;
    class  BehaviourCollideWithTerrain;

    class  BNodeDriveVehicles;

    class  BNodeGravity;
    class  BNodeTerrainCollision;

    class  CharacterFreecam;
    class  CharacterPlayerFPS;
}



// class meat::BNodeConditionMouseCaptured: public BNodeCondition
// {
// public:
//     BNodeConditionMouseCaptured();
// };



// class meat::BehaviourUserKeyInput: public BNodeAction
// {
// public:
//     using BNodeAction::BNodeAction;
//     virtual Status update( idk::EngineAPI&, meat::World& ) final;
// };


// class meat::BehaviourUserMouseInput: public BNodeAction
// {
// public:
//     using BNodeAction::BNodeAction;
//     virtual Status update( idk::EngineAPI&, meat::World& ) final;
// };


// class meat::BNodeGravity: public BNodeAction
// {
// public:
//     using BNodeAction::BNodeAction;
//     virtual Status update( idk::EngineAPI&, meat::World& ) final;
// };


// class meat::BNodeTerrainCollision: public BNodeAction
// {
// private:
//     static constexpr float dt_threshold = 1.0f / 8.0f;
//     float m_airtime = 0.0f;
// public:
//     using BNodeAction::BNodeAction;
//     virtual Status update( idk::EngineAPI&, meat::World& ) final;
// };


// class meat::BehaviourCollideWithTerrain: public BNodeAction
// {
// public:
//     using BNodeAction::BNodeAction;
//     virtual Status update( idk::EngineAPI&, meat::World& ) final;
// };




// class meat::BehaviourUserInput: public BNodeAction
// {
// private:

// public:
//     using BNodeAction::BNodeAction;
//     virtual Status update( idk::EngineAPI&, meat::World& ) final;
// };









class meat::CharacterPlayerFPS: public meat::ActorBase
{
private:
    int m_hinge_obj;

public:
    CharacterPlayerFPS( World&, const glm::vec3 &pos=glm::vec3(0), float yaw=0 );

    // virtual void update( idk::EngineAPI&, meat::World& ) override;
    virtual void move( const glm::vec3& ) override;
    virtual void look( const glm::vec2& ) override;
};

// namespace meat
// {
//     class  CharacterFreecam;
//     class  CharacterPlayerFPS;
//     class  ControllerPlayer;
// }



class meat::CharacterFreecam: public ActorBase
{
protected:
    int m_hinge_obj;

public:
    CharacterFreecam( World &world, const glm::vec3 &pos=glm::vec3(0.0f), float yaw=0.0f );

    virtual void look( const glm::vec2& ) override;
    virtual void move( const glm::vec3& ) override;
};



// class meat::CharacterPlayerFPS: public CharacterFreecam
// {
// private:
//     int m_sounds0[4];
//     int m_sounds1[4];
//     int m_sounds2[4];

// public:
//     CharacterPlayerFPS( World &world, const glm::vec3 &pos=glm::vec3(0.0f), float yaw=0.0f );

//     virtual void move( idk::EngineAPI&, const glm::vec3& ) override;
//     virtual void jump() override;

// };


// class meat::ControllerPlayer: public ControllerBase2
// {
// public:
//     virtual void update( idk::EngineAPI&, ActorBase* );
// };



