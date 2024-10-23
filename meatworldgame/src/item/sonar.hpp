#pragma once

#include "../character/base.hpp"


namespace meat
{
    class ItemSonar;
}


class meat::ItemSonar: public meat::ItemBase
{
private:
    float m_tickrate;
    float m_accum = 0.0f;

    std::queue<float> m_readings;
    glm::vec3 m_dir;


public:
    ItemSonar( ActorBase *owner, const glm::vec3 &dir );
    virtual void update( idk::EngineAPI&, meat::World& ) override;

    bool read( float *f );

};

