#include "npc.hpp"
#include "../systems/sys-player.hpp"



meatworld::CharacterMotion
meatworld::NPCController::getMovement( idk::EngineAPI &api, CharacterBase *character )
{
    using namespace idk;

    float dtime = api.dtime();
    auto &ren   = api.getRenderer();
    
    auto &events = api.getEventSys();
    auto &K      = events.keylog();


    CharacterMotion motion;

    switch (m_state)
    {
        case State::IDLE:      break;
        case State::WALKING:   break;
    }


    int character_id = character->objID();
    glm::vec3 character_pos = TransformSys::getWorldPosition(character_id);


    // if (m_state == State::IDLE)
    {
        const auto &cmp = *(ECS2::getComponentArray<PlayerCmp>().begin());
        glm::vec3 player_pos = TransformSys::getWorldPosition(cmp.obj_id);

        float dist = glm::distance(character_pos, player_pos);

        if (dist > 8.0f)
        {
            // m_state = State::WALKING;
            m_target_pos = player_pos;
        }
    }


    // if (m_state == State::WALKING)
    {
        glm::mat3 R = glm::inverse(glm::mat3(TransformSys::getModelMatrix(character_id)));
        glm::vec3 dir = R * glm::normalize(m_target_pos - character_pos);

        motion.delta = dir;
        motion.yaw = atan2(dir.y, dir.x);
    }

    // if (events.mouseCaptured() == false)
    // {
    //     return motion;
    // }

    // static const glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
    // static const glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    // static const glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

    // glm::vec3 delta  = glm::vec3(0.0f);
    // glm::vec2 dmouse = -0.1f * events.mouseDelta();

    // if (K.keyDown(idk::Keycode::A)) { delta -= right; }
    // if (K.keyDown(idk::Keycode::D)) { delta += right; }
    // if (K.keyDown(idk::Keycode::W)) { delta += front; }
    // if (K.keyDown(idk::Keycode::S)) { delta -= front; }

    // motion.run    = K.keyDown(idk::Keycode::LSHIFT);
    // motion.jump   = K.keyTapped(idk::Keycode::SPACE);
    // motion.crouch = K.keyDown(idk::Keycode::LCTRL);
    // motion.aim    = events.mouseDown(idk::MouseButton::RIGHT);
    // motion.attack = events.mouseClicked(idk::MouseButton::LEFT);

    // motion.delta = delta;
    // motion.yaw   = dmouse.x;
    // motion.pitch = dmouse.y;

    return motion;
}

