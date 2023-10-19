#include "idk_voxel.hpp"



void
idk_Voxel::init( idk::Engine &engine )
{
    idk::gl::genVertexArrays(1, &m_dummy_VAO);

    idk::ColorAttachmentConfig color_config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_FLOAT
    };


    idk::RenderEngine &ren = engine.rengine();
    idk::glFramebuffer &framebuffer = m_framebuffer;

    auto resize_callback = [&ren, &framebuffer, color_config]()
    {
        glm::ivec2 size = ren.resolution();
        framebuffer.reset(size.x, size.y, 2);
        framebuffer.colorAttachment(0, color_config);
        framebuffer.colorAttachment(1, color_config);
    };
    engine.eventManager().onWindowEvent(idk::WindowEvent::RESIZE, resize_callback);
    resize_callback();


    GLuint &shader = m_shader;
    auto shader_callback = [&shader]()
    {
        shader = idk::gltools::compileProgram("shaders/", "screenquad.vs", "SVOctree.fs");
    };
    engine.eventManager().onKeyEvent(idk::Keycode::E, idk::KeyEvent::TAPPED, shader_callback);
    shader_callback();
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
    idk::RenderEngine &ren = engine.rengine();
    svo_draw(ren, m_svo.nodes(), 0, glm::vec3(0.0f), 0, m_svo.SPAN, m_svo.MINIMUM);

    if (engine.eventManager().mouseDown(idk::MouseButton::LEFT))
    {
        auto &cam = ren.getCamera();
        glm::vec3 dir = 5.0f*cam.front();

        m_svo.insert(glm::ivec3(0), cam.transform().position() + dir);
    }


    m_framebuffer.bind();
    m_framebuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    idk::gltools::useProgram(m_shader);

    idk::gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);
    idk::gl::bindVertexArray(m_dummy_VAO);
    idk::gl::drawArrays(GL_TRIANGLES, 0, 3);
    idk::gl::bindVertexArray(0);
    idk::gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    
    m_framebuffer.unbind();

    ren.blitFramebuffer(m_framebuffer);
}

