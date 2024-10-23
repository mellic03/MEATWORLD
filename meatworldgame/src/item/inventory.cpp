#include "inventory.hpp"
#include "../world/world.hpp"

#include <libidk/idk_random.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/particle/particle.hpp>

using namespace meat;


ItemInventory::ItemInventory( ActorBase *owner, meat::World &world )
:   ItemBase(owner),
    worldref(world)
{


}


void
ItemInventory::update( idk::EngineAPI &api, meat::World& )
{

}


void
ItemInventory::open()
{
    worldref.EM.emit("open-inventory", m_owner);
}




