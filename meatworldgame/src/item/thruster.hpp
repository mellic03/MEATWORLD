#pragma once

#include "../character/base.hpp"


namespace meat
{
    class ItemThruster;
}


class meat::ItemThruster: public meat::ItemBase
{
private:
    int       m_obj;
    int       m_emitter;
    glm::vec3 m_tilt    = glm::vec3(0.0f);
    glm::vec3 m_tiltmin = glm::vec3(-1.5f);
    glm::vec3 m_tiltmax = glm::vec3(+1.5f);
    float     m_thrust  = 0.0f;
    bool      m_enabled = false;
    bool      m_first   = true;

    void create_emitter( idk::EngineAPI&, const glm::vec3&, const glm::vec3& );

public:
    ItemThruster( ActorBase *owner, const glm::vec3 &offset );

    void enabled ( bool  b ) { m_enabled  = b; }
    void thrust  ( float t ) { m_thrust   = glm::max(t, 0.0f); }

    void tilt( float pitch, float yaw )
    {
        m_tilt = glm::mix(m_tilt, glm::vec3(pitch, yaw, 0.0f), 0.25f);
        m_tilt = glm::clamp(m_tilt, m_tiltmin, m_tiltmax);
    }

    virtual void update( idk::EngineAPI&, meat::World& ) override {  };
    glm::vec3 update( idk::EngineAPI& );

};

