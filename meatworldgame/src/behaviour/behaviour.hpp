#pragma once

#include <vector>
#include <any>
#include <functional>
#include <initializer_list>
#include <iostream>


namespace idk
{
    class EngineAPI;
}

namespace meat
{
    class ActorBase;
    class World;

    class BNode;
    class BNodeComposite;
    class BNodeSequence;
    class BNodeFallback;
    class BNodeXOR;

    class BNodeNOT;
    class BNodeCondition;
    class BNodeAction;
    class BNodeActionCallback;
}


class meat::BNode
{
private:
    float m_timer = 0.0f;

public:
    enum Status { SLEEPING, RUNNING, FAILURE, SUCCESS };

    std::vector<BNode*> children;

    BNode() {  }
    BNode( BNode *child ): children(1, child) {  };
    BNode( std::initializer_list<BNode*> args ): children(args) {  };
    ~BNode();

    virtual Status update( idk::EngineAPI&, meat::World& ) = 0;

    Status updateNode( idk::EngineAPI&, meat::World& );
    void sleep( float dt ) { m_timer = dt; };
};



class meat::BNodeComposite: public BNode
{
public:
    using BNode::BNode;
    virtual Status update( idk::EngineAPI&, meat::World& );

};


class meat::BNodeSequence: public BNode
{
public:
    using BNode::BNode;
    virtual Status update( idk::EngineAPI&, meat::World& );

};


class meat::BNodeFallback: public BNode
{
public:
    using BNode::BNode;
    virtual Status update( idk::EngineAPI&, meat::World& );

};


class meat::BNodeXOR: public BNode
{
public:
    using BNode::BNode;
    virtual Status update( idk::EngineAPI&, meat::World& );

};




class meat::BNodeNOT: public BNode
{
public:
    BNodeNOT( BNode *child ): BNode(child) {  };
    virtual Status update( idk::EngineAPI&, meat::World& ) final;
};



class meat::BNodeCondition: public BNode
{
protected:
    std::function<bool()> m_fn;

public:
    BNodeCondition( const std::function<bool()> &fn ): m_fn(fn) {  };

    virtual Status update( idk::EngineAPI&, meat::World& ) final
    {
        if (m_fn())  return Status::SUCCESS;
        else         return Status::FAILURE;
    }
};



class meat::BNodeAction: public BNode
{
protected:
    std::function<Status()> m_fn;

public:
    BNodeAction( const std::function<Status()> &fn ): m_fn(fn) {  };

    virtual Status update( idk::EngineAPI&, meat::World& )
    {
        return m_fn();
    }
};




// class meat::BNodeAction: public BNode
// {
// protected:
//     ActorBase *m_actor;

// public:
//     BNodeAction( ActorBase *A ): m_actor(A) {  };

//     virtual Status update( idk::EngineAPI&, meat::World& )
//     {
//         return SUCCESS;
//     }

// };


