#include "idk_voxel.hpp"



void
idk_Voxel::init( idk::Engine &engine )
{
    m_svo = idk::SVOctree(32);
}


void
idk_Voxel::stage_A( idk::Engine &engine )
{
    // Need to provide an API where I can run a fragment shader, rendering the result to a buffer.
    // The buffer needs to be blitted onto the main one including depth during Engine::endframe().


}
