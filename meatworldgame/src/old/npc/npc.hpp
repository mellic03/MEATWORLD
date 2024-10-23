#pragma once

#include "../character/character.hpp"
#include "../character/humanoid.hpp"


namespace meatworld
{
    class NPCController;
}


class meatworld::NPCController: public meatworld::CharacterController
{
private:
    enum State: uint32_t { IDLE, WALKING };
    State m_state = State::IDLE;

    glm::vec3 m_target_pos = glm::vec3(0.0f);


public:
    virtual CharacterMotion getMovement( idk::EngineAPI&, CharacterBase* );

};

