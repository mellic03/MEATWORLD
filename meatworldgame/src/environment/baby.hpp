#pragma once


#include <IDKECS/IDKECS.hpp>

#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include "systems/sys-grabbable.hpp"




class meatworld::Baby: public meatworld::PickupBase
{
private:

public:

    Baby( const glm::vec3 &pos )
    {
        using namespace idk;

        m_obj_id = ECS2::createGameObject("Baby", false);

        ECS2::giveComponent<TransformCmp>(m_obj_id);
        ECS2::giveComponent<ModelCmp>(m_obj_id);
        ECS2::giveComponent<GrabbableCmp>(m_obj_id);

        ModelSys::assignModel(m_obj_id, "assets/models/error.idkvi");
    }

};

