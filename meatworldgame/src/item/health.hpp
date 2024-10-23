#pragma once

#include "../character/base.hpp"


namespace meat
{
    class ItemHealth;
}


class meat::ItemHealth: public ItemBase
{
public:
    ItemHealth( ActorBase *owner=nullptr )
    :   ItemBase(owner)
    {
        setModelPath("assets/models/bust.idkvi");
    }
};


