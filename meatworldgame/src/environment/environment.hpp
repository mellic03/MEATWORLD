#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <string>


namespace meatworld
{
    class iInteractive;

    class iTalkable;
    class iPickup;

    class ItemBase;
    class PickupBase;

    class Baby;
}



class meatworld::iInteractive
{
private:
    std::string m_name;
    std::string m_model_path;

protected:
    int m_obj_id = -1;

public:

    virtual const std::string &onScreenPrompt() = 0;
    virtual void onInteract() = 0;
};



