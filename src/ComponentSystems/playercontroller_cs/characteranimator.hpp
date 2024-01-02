#pragma once

#include <IDKGameEngine/IDKengine.hpp>


namespace idkg
{
    class CharacterAnimator;
};



class idkg::CharacterAnimator
{
private:
    glm::vec3 m_bearing    = glm::vec3(0.0f);
    glm::vec4 m_thresholds = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);

public:

    const static int ANIM_IDLE = 0;
    const static int ANIM_WALK = 3;
    const static int ANIM_RUN  = 1;

    glm::vec3 getMotion( float speed, const glm::vec3 &dir, idk::Animator & );
};
