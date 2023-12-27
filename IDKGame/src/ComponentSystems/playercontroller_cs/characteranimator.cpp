#include "characteranimator.hpp"


glm::vec3
idkg::CharacterAnimator::getMotion( float           alpha,
                                    const glm::vec3 &dir,
                                    idk::Animator   &animator )
{
    glm::vec3 rootmotion = animator.rootMotion();
    rootmotion.y *= 0.0f;

    int speed = glm::floor(alpha);
    alpha -= speed;


    if (speed < 1)
    {
        animator.setAnimA(1);
        animator.setAnimB(4);
    }

    else if (speed < 2)
    {
        animator.setAnimA(4);
        animator.setAnimB(0);
    }

    else
    {
        animator.setAnimA(0);
        animator.setAnimB(2); 
    }


    alpha = glm::clamp(alpha, 0.0f, 1.0f);
    animator.setBlendAlpha(alpha);


    return glm::length(rootmotion) * glm::normalize(dir);
}

