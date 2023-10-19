#include "idk_voxel.hpp"



void
idk_Voxel::init( idk::Engine &engine )
{
    m_svo.insert(glm::ivec3(0, 255, 0), glm::vec3(10.0f, 1.0f, -3.0f));

    auto ree = idk::glFramebufferNew(1000, 1000);

}



void
svo_draw( idk::RenderEngine &ren, idk::Allocator<idk::SVOctree::Node> &nodes,
          int id, glm::vec3 center, int depth, float span, float minimum )
{
    idk::SVOctree::Node &node = nodes.get(id);

    if (span <= minimum)
    {
        idk::Transform transform;
        transform.translate(center);
        transform.scale(glm::vec3(span));
        ren.drawWireframe(ren.solid_shader, ren.CUBE_PRIMITIVE, transform);

        return;
    }

    idk::Transform transform;
    transform.translate(center);
    transform.scale(glm::vec3(span));
    ren.drawWireframe(ren.solid_shader, ren.CRATE_PRIMITIVE, transform);


    for (int i=0; i<8; i++)
    {
        if (node.children[i] != -1)
        {
            const glm::vec3 new_center = idk::SVOctree::shift_center(i, center, span);
            svo_draw(ren, nodes, node.children[i], new_center, depth+1, span/2.0f, minimum);
        }
    }
};



void
idk_Voxel::stage_A( idk::Engine &engine )
{
    // Need to provide an API where I can run a fragment shader, rendering the result to a buffer.
    // The buffer needs to be blitted onto the main one including depth during Engine::endframe().

    idk::RenderEngine &ren = engine.rengine();
    svo_draw(ren, m_svo.nodes(), 0, glm::vec3(0.0f), 0, m_svo.SPAN, m_svo.MINIMUM);

    if (engine.eventManager().mouseDown(idk::MouseButton::LEFT))
    {
        auto &cam = ren.getCamera();
        glm::vec3 dir = 5.0f*cam.front();

        m_svo.insert(glm::ivec3(0), cam.transform().position() + dir);
    }

}

