#include "behaviour.hpp"
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <iostream>

using namespace meat;



BNode::~BNode()
{
    for (auto *child: children)
    {
        delete child;
    }
}



BNode::Status
BNode::updateNode( idk::EngineAPI &api, meat::World &world )
{
    m_timer -= api.dtime();

    if (m_timer > 0.0f)
    {
        return SLEEPING;
    }

    return update(api, world);
}


BNode::Status
BNodeComposite::update( idk::EngineAPI &api, meat::World &world )
{
    int fail_count = 0;

    for (BNode *child: children)
    {
        child->updateNode(api, world);
    }

    return SUCCESS;
}


BNode::Status
BNodeSequence::update( idk::EngineAPI &api, meat::World &world )
{
    int success_count = 0;

    for (BNode *child: children)
    {
        auto res = child->updateNode(api, world);

        if (res == SUCCESS)
        {
            success_count += 1;
        }

        else if (res == FAILURE)
        {
            return res;
        }
    }

    if (success_count == children.size())
    {
        return SUCCESS;
    }

    return RUNNING;
}


BNode::Status
BNodeFallback::update( idk::EngineAPI &api, meat::World &world )
{
    int fail_count = 0;

    for (BNode *child: children)
    {
        auto res = child->updateNode(api, world);

        if (res == SUCCESS)
        {
            return SUCCESS;
        }

        else if (res == FAILURE)
        {
            fail_count += 1;
        }
    }

    if (fail_count == children.size())
    {
        return FAILURE;
    }

    return RUNNING;
}


BNode::Status
BNodeXOR::update( idk::EngineAPI &api, meat::World &world )
{
    int success_count = 0;
    auto res = RUNNING;

    for (BNode *child: children)
    {
        res = child->updateNode(api, world);

        if (res == SUCCESS)
        {
            success_count += 1;
        }
    }

    if (success_count != 1)
    {
        return FAILURE;
    }

    return SUCCESS;
}


BNode::Status
BNodeNOT::update( idk::EngineAPI &api, meat::World &world )
{
    auto res = children[0]->updateNode(api, world);

    if (res == SUCCESS)
        res = FAILURE;
    else
        res = SUCCESS;
        
    return res;
}
