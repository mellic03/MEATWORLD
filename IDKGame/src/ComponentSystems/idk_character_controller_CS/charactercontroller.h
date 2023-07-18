#pragma once

#include "../IDK_builtin_CS_common.h"


class CharacterController
{
private:

public:

    void    move( glm::vec3 v );
    void    rotateTowards( glm::vec3 target, float alpha );

    std::function<void(int, idk::Engine &, CharacterController &)>      controlMethod;
    int     obj_id;
};
