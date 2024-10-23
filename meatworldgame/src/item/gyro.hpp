#pragma once

#include "../character/base.hpp"


namespace meat
{
    class ItemGyro;
}


class meat::ItemGyro: public meat::ItemBase
{
private:
    float m_tickrate = 1.0f / 4.0f;
    float m_accum    = 0.0f;
    float m_reading  = 0.0f;
    bool  m_ready    = false;

public:
    ItemGyro( ActorBase *owner );
    virtual void update( idk::EngineAPI&, meat::World& ) override;

    bool read( float *f );
};

