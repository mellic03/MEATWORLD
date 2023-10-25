#include "idk_voxel.hpp"
#include <chrono>


void
idk_Voxel::init( idk::Engine &engine )
{
    m_shader.load("shaders/", "screenquad.vs", "SVOctree.fs");
    m_whitenoise = idk::noisegen2D::white(512, 512);

    idk::ColorAttachmentConfig color_config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_FLOAT
    };

    idk::RenderEngine &ren = engine.rengine();
    idk::glFramebuffer &framebuffer = m_framebuffer;
    idk::glFramebuffer &framebuffer_2 = m_framebuffer_2;
    idk::glFramebuffer &gbuffer = m_gbuffer;

    auto resize_callback = [&ren, &framebuffer, &framebuffer_2, &gbuffer, color_config]()
    {
        glm::ivec2 size = ren.resolution();
        framebuffer.reset(size.x/2, size.y/2, 2);
        framebuffer.colorAttachment(0, color_config);
        framebuffer.colorAttachment(1, color_config);

        framebuffer_2.reset(size.x/8, size.y/8, 2);
        framebuffer_2.colorAttachment(0, color_config);
        framebuffer_2.colorAttachment(1, color_config);

        gbuffer.reset(size.x, size.y, 4);
        gbuffer.colorAttachment(0, color_config);
        gbuffer.colorAttachment(1, color_config);
        gbuffer.colorAttachment(2, color_config);
        gbuffer.colorAttachment(3, color_config);
    };
    engine.eventManager().onWindowEvent(idk::WindowEvent::RESIZE, resize_callback);
    resize_callback();



    idk::glShader &shader = m_shader;
    auto shader_callback = [&shader]()
    {
        shader.compile();
    };
    engine.eventManager().onKeyEvent(idk::Keycode::E, idk::KeyEvent::TAPPED, shader_callback);
    shader_callback();


    for (int z=-15; z<16; z++)
    {
        for (int y=-15; y<-0.5; y++)
        {
            for (int x=-15; x<16; x++)
            {
                glm::vec3 pos = glm::vec3(x, y, z);
                m_svo.insert(3, pos, 2.0f);
            }
        }
    }

    for (float z=-15; z<16; z+=0.5)
    {
        for (float y=-0.5; y<-0.2; y+=0.5)
        {
            for (float x=-15; x<16; x+=0.5)
            {
                glm::vec3 pos = glm::vec3(x, y, z);
                m_svo.insert(2, pos, 1.0f);
            }
        }
    }


    for (float z=-15; z<16; z+=0.5)
    {
        for (float y=-0.5; y<0.5; y+=0.5)
        {
            for (float x=-15; x<16; x+=0.5)
            {
                glm::vec3 pos = glm::vec3(x, y, z);
                m_svo.insert(1, pos, 1.0f);
            }
        }
    }


    m_SSBO.init(3);
    m_SSBO.bufferData(m_svo.nodegroups().bytesize(), m_svo.nodegroups().data());
}



void
idk_Voxel::render( idk::Engine &engine, idk::glFramebuffer framebuffer )
{
    idk::RenderEngine &ren = engine.rengine();

    // First draw background quad to make creating rays easier
    // -----------------------------------------------------------------------------------------
    m_gbuffer.bind();
    m_gbuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f * ren.getCamera().farPlane()));
    modelmat = glm::scale(modelmat, glm::vec3(300.0f, 300.0f, 1.0f));

    ren.m_background_shader.bind();
    ren.m_background_shader.set_mat4("un_model", modelmat);

    idk::gl::bindVertexArray(ren.m_quad_VAO);
    idk::gl::drawArrays(GL_TRIANGLES, 0, 6);

    m_gbuffer.unbind();
    // -----------------------------------------------------------------------------------------

    static float increment = 0.0f;

    increment += 0.0001f;
    if (increment >= 1.0f)
    {
        increment = 0.0f;
    }


    framebuffer.bind();
    framebuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();
    m_shader.set_sampler2D("un_texture_1", m_gbuffer.attachments[1]);
    m_shader.set_sampler2D("un_whitenoise", m_whitenoise);
    m_shader.set_float("un_span", m_svo.SPAN);
    m_shader.set_float("un_increment", increment);



    idk::gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);
    idk::gl::bindVertexArray(ren.m_quad_VAO);
    idk::gl::drawArrays(GL_TRIANGLES, 0, 6);
    idk::gl::bindVertexArray(0);
    idk::gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    
    framebuffer.unbind();

}



void idk_Voxel::place( idk::Engine &engine, int blocktype, int ksize, float span )
{
    auto &cam = engine.rengine().getCamera();
    glm::vec3 dir = glm::normalize(cam.front());

    glm::vec3 pos = cam.transform().position();

    glm::vec3 pos1 = pos + 1.0f*dir;
    glm::vec3 hitpoint = m_svo.hitpoint(pos, dir);

    // if (glm::distance(pos, hitpoint) < glm::distance(pos, pos1))
    // {
    //     pos = hitpoint;
    // }

    // else
    {
        pos = pos1;
    }


    if (pos != glm::vec3(0.0f))
    {
        if (ksize == 1)
        {
            m_svo.insert(blocktype, pos, 2.0f*span);
        }

        else
        for (float z=pos.z-ksize*span; z<pos.z+ksize*span; z+=span)
        {
            for (float y=pos.y-ksize*span; y<pos.y+ksize*span; y+=span)
            {
                for (float x=pos.x-ksize*span; x<pos.x+ksize*span; x+=span)
                {
                    if (glm::distance(glm::vec3(x, y, z), pos) > ksize*span)
                        continue;

                    glm::vec3 ipos = pos;
                    ipos.x = x;
                    ipos.y = y;
                    ipos.z = z;
                    m_svo.insert(blocktype, ipos, 2.0f*span);
                }
            }
        }

        // auto &svo = m_svo;
        // m_svo.nodegroups().for_each(
        //     [&svo](int id, idk::SVOctree::Nodes &nodes)
        //     {
        //         for (int i=0; i<8; i++)
        //         {
        //             svo.node(id, i).irradiance.x /= 4.0f;
        //             svo.node(id, i).irradiance.y /= 4.0f;
        //             svo.node(id, i).irradiance.z /= 4.0f;
        //             svo.node(id, i).irradiance.w = 1.0f;
        //         }
        //     }
        // );
    }


    m_SSBO.bufferData(m_svo.nodegroups().bytesize(), m_svo.nodegroups().data());

    std::cout << "KB: " << m_svo.nodegroups().bytesize() / 1000 << "\n";
}

void
idk_Voxel::stage_A( idk::Engine &engine )
{
    idk::RenderEngine &ren = engine.rengine();

    static int blocktype = 1;
    static int ksize = 8;
    static float span = 0.0625f; //1.0f / 4.0f;

    if (engine.eventManager().keylog().keyTapped(idk::Keycode::F))
    {
        blocktype = (blocktype + 1) % 7;
        std::cout << "blocktype: " << blocktype << std::endl;
    }

    if (engine.eventManager().keylog().keyTapped(idk::Keycode::U))
    {
        ksize /= 2;
        std::cout << "ksize = " << ksize << "\n";
    }

    if (engine.eventManager().keylog().keyTapped(idk::Keycode::J))
    {
        ksize *= 2;
        std::cout << "ksize = " << ksize << "\n";
    }

    if (engine.eventManager().keylog().keyTapped(idk::Keycode::I))
    {
        span *= 2.0f;
        std::cout << "span = " << span << "\n";
    }

    if (engine.eventManager().keylog().keyTapped(idk::Keycode::K))
    {
        span /= 2.0f;
        std::cout << "span = " << span << "\n";
    }


    static bool can_place = true;

    if (can_place && engine.eventManager().mouseDown(idk::MouseButton::LEFT))
    {
        place(engine, blocktype, ksize, span);
        can_place = false;
    }
    else if (engine.eventManager().mouseUp(idk::MouseButton::LEFT))
    {
        can_place = true;
    }

    if (engine.eventManager().mouseDown(idk::MouseButton::RIGHT))
    {
        place(engine, 0, ksize, span);
    }


    m_shader.bind();

    m_shader.set_float("un_f_sign", -1.0f);
    this->render(engine, m_framebuffer_2);

    m_shader.set_float("un_f_sign", +1.0f);
    this->render(engine, m_framebuffer);

    ren.blitFramebuffer(m_framebuffer);
    m_shader.unbind();


    void *gpudata = m_SSBO.mapBuffer();
    void *cpudata = m_svo.nodegroups().data();
    memcpy(cpudata, gpudata, m_svo.nodegroups().bytesize());
    m_SSBO.unmapBuffer();

}



