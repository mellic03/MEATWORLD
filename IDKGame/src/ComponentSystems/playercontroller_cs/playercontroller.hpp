#pragma once

#include <IDKengine/IDKengine.hpp>
#include "characteranimator.hpp"


namespace idkg
{
    class CharacterController;
};



class idkg::CharacterController
{
private:
    int        m_obj_id;

    glm::vec3  bearing = glm::vec3(0.0f, 0.0f, -1.0f);

    idkg::CharacterAnimator m_animator;


public:
            CharacterController( int obj_id ): m_obj_id(obj_id) {  };

    void    update( idk::Engine & );

};


