#include "world.hpp"

using namespace idk;



bool
meatworld::GameWorld::raycastDown( const glm::vec3 &origin, glm::vec3 *res )
{
    glm::vec3 res0, res1;

    // bool cast0 = PhysicsSys::raycast(origin, glm::vec3(0.0f, -1.0f, 0.0f), res0);

    res1   = origin;
    res1.y = idk::TerrainRenderer::heightQuery(origin.x, origin.z);

    if (res)
    {
        *res = res1;
    }

    return true;
}


