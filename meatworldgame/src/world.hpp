#pragma once


#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKBuiltinCS/sys-transform.hpp>
#include <IDKBuiltinCS/sys-physics.hpp>
#include <IDKBuiltinCS/sys-model.hpp>
#include <IDKBuiltinCS/sys-particle.hpp>

#include <IDKGraphics/terrain/terrain.hpp>


namespace meatworld
{
    class GameWorld;
};


class meatworld::GameWorld
{
private:

public:
    static bool raycastDown( const glm::vec3 &origin, glm::vec3 *res = nullptr );
};

