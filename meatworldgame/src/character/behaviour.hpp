#pragma once

#include "base.hpp"


namespace meat
{
    // class BehaviourRenderSphere;

    class BehaviourGravity;
    class BehaviourCollideWithTerrain;
    class BehaviourPushBodies;

    // class BehaviourWeaponSlot;
    // class BehaviourLimbs;

    // class BehaviourMoveToPlayer;
    class BehaviourPlayerKeyInput;
    class BehaviourPlayerMouseInput;
    class BehaviourPlayerZoom;
}


// class meat::BehaviourRenderSphere: public BehaviourBase
// {
// public:
//     using BehaviourBase::BehaviourBase;
//     virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
// };



class meat::BehaviourGravity: public BehaviourBase
{
private:

public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};


class meat::BehaviourCollideWithTerrain: public BehaviourBase
{
private:
    static constexpr float dt_threshold = 1.0f / 8.0f;
    float m_airtime = 0.0f; 

public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};


class meat::BehaviourPushBodies: public BehaviourBase
{
public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};


// // class meat::BehaviourCollideWithWater: public BehaviourBase
// // {
// // public:
// //     virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
// // };






// class meat::BehaviourLimbs: public BehaviourBase
// {
// private:
//     std::vector<glm::vec3> m_joints = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
//     std::vector<float> m_dists = { 1.0f, 1.5f };
//     glm::vec3 m_prev = glm::vec3(0.0f);

// public:
//     using BehaviourBase::BehaviourBase;
//     virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
// };


// class meat::BehaviourMoveToPlayer: public BehaviourBase
// {
// private:
//     bool m_moving = false;
//     glm::vec3 m_target = glm::vec3(0.0f);

// public:
//     using BehaviourBase::BehaviourBase;
//     virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
// };


class meat::BehaviourPlayerKeyInput: public BehaviourBase
{
public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};


class meat::BehaviourPlayerMouseInput: public BehaviourBase
{
public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};


class meat::BehaviourPlayerZoom: public BehaviourBase
{
private:
    int m_zoomlevel = 0;

public:
    using BehaviourBase::BehaviourBase;
    virtual void update( idk::EngineAPI&, meat::World&, ActorBase* ) final;
};




