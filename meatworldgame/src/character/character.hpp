#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <glm/glm.hpp>

#include "base.hpp"


namespace idk
{
    class EngineAPI;
}


namespace meat
{
    // class  BehaviourBase2;
    // class  ActorBase;
    class  ControllerBase2;
    class  VehicleBase;

    class  CharacterManager;
}



class meat::ControllerBase2
{
public:
    virtual void update( idk::EngineAPI&, ActorBase* ) = 0;
};


