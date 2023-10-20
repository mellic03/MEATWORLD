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
    idk::glFramebuffer &gbuffer = m_gbuffer;

    auto resize_callback = [&ren, &framebuffer, &gbuffer, color_config]()
    {
        glm::ivec2 size = ren.resolution();
        framebuffer.reset(size.x, size.y, 2);
        framebuffer.colorAttachment(0, color_config);
        framebuffer.colorAttachment(1, color_config);

        gbuffer.reset(size.x, size.y, 4);
        gbuffer.colorAttachment(0, color_config);
        gbuffer.colorAttachment(1, color_config);
        gbuffer.colorAttachment(2, color_config);
        gbuffer.colorAttachment(3, color_config);
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


    m_SSBO.init();
    m_svo.insert(1, glm::vec3(1.5f, 1.0f, 2.5f));
    m_SSBO.bufferData(m_svo.nodes().size() * sizeof(idk::SVOctree::Node), m_svo.nodes().data());
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
    // svo_draw(ren, m_svo.nodes(), 0, glm::vec3(0.0f), 0, m_svo.SPAN, m_svo.MINIMUM);

    static int blocktype = 1;

    if (engine.eventManager().mouseDown(idk::MouseButton::LEFT))
    {
        auto &cam = ren.getCamera();
        glm::vec3 dir = 5.0f*cam.front();

        glm::vec3 pos = cam.transform().position() + dir;

        for (int z=0; z<2; z++)
        {
            for (int y=0; y<2; y++)
            {
                for (int x=0; x<2; x++)
                {
                    glm::vec3 ipos = pos;
                    ipos.x += x * (1.0f / 4.0f);
                    ipos.y += y * (1.0f / 4.0f);
                    ipos.z += z * (1.0f / 4.0f);
                    m_svo.insert(blocktype, ipos);
                }
            }
        }

        m_SSBO.bufferData(m_svo.nodes().bytesize(), m_svo.nodes().data());
        std::cout << "SIZE: " << m_svo.nodes().bytesize() << "\n";
    }
   
    if (engine.eventManager().keylog().keyTapped(idk::Keycode::F))
    {
        blocktype = (blocktype == 1) ? 2 : 1;
    }


    // First draw background quad to make creating rays easier
    // -----------------------------------------------------------------------------------------
    m_gbuffer.bind();
    m_gbuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    idk::gl::bindVertexArray(ren.m_quad_VAO);
    idk::gltools::useProgram(ren.m_background_shader);
    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f * ren.getCamera().farPlane()));
    modelmat = glm::scale(modelmat, glm::vec3(300.0f, 300.0f, 1.0f));
    idk::gltools::setUniform_mat4("un_model", modelmat);
    idk::gl::drawArrays(GL_TRIANGLES, 0, 6);

    m_gbuffer.unbind();
    // -----------------------------------------------------------------------------------------


    m_framebuffer.bind();
    m_framebuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    idk::gltools::useProgram(m_shader);

    idk::gltools::setUniform_float("un_fov", 90.0f);
    idk::gltools::setUniform_vec2("un_resolution", ren.resolution());
    idk::gltools::setUniform_vec3("un_viewdir", ren.getCamera().front());
    idk::gltools::setUniform_texture("un_texture_1", m_gbuffer.attachments[1]);

    idk::gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);
    idk::gl::bindVertexArray(ren.m_quad_VAO);
    idk::gl::drawArrays(GL_TRIANGLES, 0, 6);
    idk::gl::bindVertexArray(0);
    idk::gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    
    m_framebuffer.unbind();

    ren.blitFramebuffer(m_framebuffer);
}

