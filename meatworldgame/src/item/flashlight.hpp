#pragma once

#include "../character/base.hpp"

namespace meat
{
    class ItemFlashlight;
}


class meat::ItemFlashlight: public ItemBase
{
private:
    int m_obj;
    bool m_enabled = false;

public:
    ItemFlashlight( ActorBase *owner );
    ~ItemFlashlight();

    virtual void update( idk::EngineAPI&, meat::World& );

    void on();
    void off();
    void toggle();
};

