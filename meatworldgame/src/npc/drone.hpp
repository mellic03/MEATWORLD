#pragma once

#include "../character/base.hpp"
#include "../character/character.hpp"
#include "../vehicle/vehicle.hpp"
#include "../behaviour/behaviour.hpp"
#include "../item/thruster.hpp"

#include <libidk/idk_signal.hpp>


namespace meat
{
    class BNodeMoveTo;
    class BNodeReachTargetAngle;
    class BNodeReachTargetHeight;

    class CharacterDrone;
}

class meat::BNodeReachTargetAngle: public BNodeAction
{
private:
    ActorBase *m_actor;

public:
    BNodeReachTargetAngle( ActorBase *actor )
    :   BNodeAction([](){ return Status::SUCCESS; }),
        m_actor(actor) {  };
    virtual Status update( idk::EngineAPI&, meat::World& );
};



class meat::BNodeReachTargetHeight: public BNodeAction
{
private:
    ActorBase *m_actor;

public:
    BNodeReachTargetHeight( ActorBase *actor )
    :   BNodeAction([](){ return Status::SUCCESS; }),
        m_actor(actor) {  };
    virtual Status update( idk::EngineAPI&, meat::World& );
};



class meat::CharacterDrone: public ActorBase
{
private:
    friend class BNodeReachTargetHeight;

    meat::ItemThruster *m_thrusters[4];
    float m_target_y = 8.0f;
    idk::PID m_PID;

public:
    CharacterDrone( World &world, const glm::vec3 &pos=glm::vec3(0.0f), float yaw=0.0f );

    void setTargetHeight( float y ) { m_target_y  = y; };
    void addTargetHeight( float y ) { m_target_y += y; };

    virtual void update( idk::EngineAPI&, meat::World& ) override;
    virtual void look( const glm::vec2& );
    virtual void move( const glm::vec3& );
    virtual void moveTo( const glm::vec3& );

};


