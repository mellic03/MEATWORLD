// #include "idk_voxel.hpp"
// #include <chrono>

// #define GOLDEN_RATIO 1.618033988749
// #define EULER 2.718281828459


// int angel_model;


// idk::SVOctree
// idk_Voxel::voxelize( idk::Engine &engine, int model_id )
// {
//     // idk::RenderEngine &ren = engine.RenderEngine();

//     // idk::glShader &program = ren.getProgram("voxelize");
//     // idk::Transform transform(glm::mat4(1.0f));


//     // Create orthographic projection matrix
//     // -------------------------------------
//     // glm::mat4 projection = glm::ortho(-3.0, +3.0, -3.0, +3.0);
//     // -------------------------------------

//     // program.set_mat4("un_projee", projection);
//     // ren.drawModel_now(program, model_id, transform);


//     idk::SVOctree octree(64.0, 1.0);


//     return octree;
// }


// void
// idk_Voxel::init( idk::EngineAPI &api )
// {
//     idk::RenderEngine &ren = engine.RenderEngine();
//     ren.createProgram(m_shader_name, "shaders/", "screenquad.vs", "SVOctree.fs");


//     // ren.createProgram("voxelize", "shaders/", "voxelize.vs", "voxelize.fs");
//     // angel_model = ren.modelSystem().loadOBJ("assets/models/", "angel.obj", "angel.mtl");


//     m_whitenoise = idk::noisegen3D::white(512, 512, 64);

//     idk::glTextureConfig color_config = {
//         .internalformat = GL_RGBA32F,
//         .minfilter      = GL_LINEAR,
//         .magfilter      = GL_LINEAR,
//         .wrap_s         = GL_CLAMP_TO_EDGE,
//         .wrap_t         = GL_CLAMP_TO_EDGE,
//         .datatype       = GL_FLOAT
//     };

//     auto &framebuffers = m_framebuffers;
//     auto &gbuffers = m_gbuffers;

//     auto resize_callback = [&ren, &framebuffers, &gbuffers, color_config]()
//     {
//         glm::ivec2 size = ren.resolution();

//         framebuffers[0].reset(size.x/2, size.y/2, 2);
//         framebuffers[0].colorAttachment(0, color_config);
//         framebuffers[0].colorAttachment(1, color_config);
//         framebuffers[0].colorAttachment(2, color_config);

//         framebuffers[1].reset(size.x/2, size.y/2, 2);
//         framebuffers[1].colorAttachment(0, color_config);
//         framebuffers[1].colorAttachment(1, color_config);
//         framebuffers[1].colorAttachment(2, color_config);

//         gbuffers[0].reset(size.x/2, size.y/2, 4);
//         gbuffers[0].colorAttachment(0, color_config);
//         gbuffers[0].colorAttachment(1, color_config);
//         gbuffers[0].colorAttachment(2, color_config);
//         gbuffers[0].colorAttachment(3, color_config);
//     };
//     engine.eventManager().onWindowEvent(idk::WindowEvent::RESIZE, resize_callback);
//     resize_callback();



//     for (int z=-15; z<16; z++)
//     {
//         for (int y=-15; y<-0.5; y++)
//         {
//             for (int x=-15; x<16; x++)
//             {
//                 glm::vec3 pos = glm::vec3(x, y, z);
//                 m_svo.insert(3, pos, 2.0f);
//             }
//         }
//     }

//     for (float z=-15; z<16; z+=0.5)
//     {
//         for (float y=-0.5; y<-0.2; y+=0.5)
//         {
//             for (float x=-15; x<16; x+=0.5)
//             {
//                 glm::vec3 pos = glm::vec3(x, y, z);
//                 m_svo.insert(2, pos, 1.0f);
//             }
//         }
//     }


//     for (float z=-15; z<16; z+=0.5)
//     {
//         for (float y=-0.5; y<0.5; y+=0.5)
//         {
//             for (float x=-15; x<16; x+=0.5)
//             {
//                 glm::vec3 pos = glm::vec3(x, y, z);
//                 m_svo.insert(1, pos, 1.0f);
//             }
//         }
//     }


//     m_SSBO.init(3);
//     m_SSBO.bufferData(m_svo.nodegroups().bytesize(), m_svo.nodegroups().data());
// }



// void
// idk_Voxel::render_quad( idk::Engine &engine )
// {
//     idk::RenderEngine &ren = engine.RenderEngine();

//     glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f * ren.getCamera().farPlane()));
//     modelmat = glm::scale(modelmat, glm::vec3(300.0f, 300.0f, 1.0f));

//     idk::glShader &background = ren.getProgram("background");
//     background.bind();
//     background.set_mat4("un_model", modelmat);

//     idk::gl::bindVertexArray(ren.m_quad_VAO);
//     idk::gl::drawArrays(GL_TRIANGLES, 0, 6);
//     background.unbind();
// }


// void
// idk_Voxel::render( idk::Engine &engine, idk::glFramebuffer framebuffer )
// {
//     idk::RenderEngine &ren = engine.RenderEngine();

//     framebuffer.bind();
//     framebuffer.clear(GL_DEPTH_BUFFER_BIT);

//     idk::gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);

//     idk::gl::bindVertexArray(ren.m_quad_VAO);
//     idk::gl::drawArrays(GL_TRIANGLES, 0, 6);

//     idk::gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    
//     framebuffer.unbind();
// }



// void idk_Voxel::place( idk::Engine &engine, int blocktype, int ksize, float span )
// {
//     auto &cam = engine.RenderEngine().getCamera();
//     glm::vec3 dir = glm::normalize(cam.front());

//     glm::vec3 pos = cam.position();
//     pos = pos + 3.0f * (float)ksize * span * dir;

//     // glm::vec3 pos1 = pos + 1.0f*dir;
//     // glm::vec3 hitpoint = m_svo.hitpoint(pos, dir);

//     size_t svo_size = m_svo.nodegroups().bytesize();

//     if (pos != glm::vec3(0.0f))
//     {
//         if (ksize == 1)
//         {
//             m_svo.insert(blocktype, pos, 2.0f*span);
//         }

//         else
//         for (float z=pos.z-ksize*span; z<pos.z+ksize*span; z+=span)
//         {
//             for (float y=pos.y-ksize*span; y<pos.y+ksize*span; y+=span)
//             {
//                 for (float x=pos.x-ksize*span; x<pos.x+ksize*span; x+=span)
//                 {
//                     if (glm::distance(glm::vec3(x, y, z), pos) > ksize*span)
//                         continue;

//                     glm::vec3 ipos = pos;
//                     ipos.x = x;
//                     ipos.y = y;
//                     ipos.z = z;

//                     m_svo.insert(blocktype, ipos, 2.0f*span);
//                 }
//             }
//         }

//         // Need to copy entire buffer if octree was reallocated.
//         if (svo_size != m_svo.nodegroups().bytesize())
//         {
//             m_SSBO.bufferData(m_svo.nodegroups().bytesize(), m_svo.nodegroups().data());
//             std::cout << "KB: " << m_svo.nodegroups().bytesize() / 1000 << "\n";
//         }

//         // Otherwise only copy changed nodes
//         else
//         {
//             auto &changes = m_svo.getChanges();

//             idk::SVOctree::Nodes *gpudata = (idk::SVOctree::Nodes *)m_SSBO.mapBuffer();
//             idk::SVOctree::Nodes *cpudata = (idk::SVOctree::Nodes *)m_svo.nodegroups().data();

//             for (int groupid: changes)
//             {
//                 gpudata[groupid] = cpudata[groupid];
//             }

//             m_SSBO.unmapBuffer();
//             std::cout << "Changes: " << changes.size() << "\n";
//             m_svo.clearChanges();
//         }

//     }
// }




// void
// idk_Voxel::stage_A( idk::EngineAPI &api )
// {
//     idk::RenderEngine &ren = engine.RenderEngine();

//     static int blocktype = 0;
//     static int ksize = 8;
//     static float span = 0.0625f; //1.0f / 4.0f;

//     if (engine.eventManager().keylog().keyTapped(idk::Keycode::F))
//     {
//         blocktype = (blocktype + 1) % 8;
//         std::cout << "blocktype: " << blocktype << std::endl;
//     }

//     if (engine.eventManager().keylog().keyTapped(idk::Keycode::U))
//     {
//         ksize /= 2;
//         if (ksize == 0)
//         {
//             ksize = 1;
//         }
//         std::cout << "ksize = " << ksize << "\n";
//     }

//     if (engine.eventManager().keylog().keyTapped(idk::Keycode::J))
//     {
//         ksize *= 2;
//         std::cout << "ksize = " << ksize << "\n";
//     }

//     if (engine.eventManager().keylog().keyTapped(idk::Keycode::I))
//     {
//         span *= 2.0f;
//         std::cout << "span = " << span << "\n";
//     }

//     if (engine.eventManager().keylog().keyTapped(idk::Keycode::K))
//     {
//         span /= 2.0f;
//         std::cout << "span = " << span << "\n";
//     }



//     if (engine.eventManager().mouseDown(idk::MouseButton::LEFT))
//     {
//         place(engine, blocktype, ksize, span);
//     }

//     if (engine.eventManager().mouseDown(idk::MouseButton::RIGHT))
//     {
//         place(engine, 0, ksize, span);
//     }
    
//     static int idx = 0;
//     static glm::vec3 last_viewpos = glm::vec3(1.0f);
//     static glm::mat4 last_view = glm::mat4(1.0f);
//     static int count = 0;


//     m_gbuffers[0].bind();
//     m_gbuffers[0].clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     this->render_quad(engine);
//     m_gbuffers[0].unbind();

//     static float increment = 0.0f;
//     increment += GOLDEN_RATIO * (1.0f / 128.0f);

//     if (increment > 1.0f)
//     {
//         increment -= 1.0f;
//     }

//     idk::glShader &SVOprogram = ren.getProgram(m_shader_name);
//     SVOprogram.bind();

//     SVOprogram.set_sampler2D("un_current_position", m_gbuffers[0].attachments[1]);
//     SVOprogram.set_sampler2D("un_last_color",       m_framebuffers[(idx + 1) % 2].attachments[0]);
//     SVOprogram.set_sampler2D("un_last_depth",       m_framebuffers[(idx + 1) % 2].attachments[1]);
//     SVOprogram.set_sampler2D("un_last_position",    m_framebuffers[(idx + 1) % 2].attachments[2]);
//     SVOprogram.set_vec3("un_last_viewpos",          last_viewpos);
//     SVOprogram.set_mat4("un_last_view",             last_view);

//     SVOprogram.set_sampler3D("un_whitenoise", m_whitenoise);
//     SVOprogram.set_float("un_span", m_svo.SPAN);
//     SVOprogram.set_float("un_increment", increment);
//     SVOprogram.set_float("un_w", (float)ren.width());
//     SVOprogram.set_float("un_h", (float)ren.height());
//     SVOprogram.set_float("un_f_sign", +1.0f);



//     this->render(engine, m_framebuffers[idx]);
//     SVOprogram.unbind();


//     // this->voxelize(engine, angel_model);


//     // idk::glShader &gaussian = ren.getProgram("gaussian");
//     // gaussian.bind();
//     // idk::gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);

//     // idk::gl::bindVertexArray(ren.m_quad_VAO);
//     // idk::RenderEngine::tex2tex(gaussian, m_framebuffers[idx], m_framebuffers[(idx + 1) % 2]);
    
//     // idk::gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
//     // gaussian.unbind();

//     ren.blitFramebuffer(m_framebuffers[idx]);


//     last_viewpos = ren.getCamera().position();
//     last_view = ren.getCamera().view();

//     idx = (idx + 1) % 2;
// }



