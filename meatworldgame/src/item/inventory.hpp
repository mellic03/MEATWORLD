#pragma once
#include "../character/base.hpp"

namespace meat
{
    class ItemInventory;
}


class meat::ItemInventory: public meat::ItemBase
{
private:
    meat::World &worldref;

public:
    std::vector<std::vector<ItemBase*>> items;

    ItemInventory( ActorBase*, meat::World& );
    virtual void update( idk::EngineAPI&, meat::World& ) override;

    void open();

};

