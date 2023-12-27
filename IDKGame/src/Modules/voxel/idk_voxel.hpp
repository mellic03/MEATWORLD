// #pragma once

// #include "IDKengine/IDKengine.hpp"
// #include "idk_svoctree.hpp"

// class idk_Voxel: public idk::Module
// {
// private:
//     idk::SVOctree       m_svo = idk::SVOctree(32.0f, 1.0f);
//     idk::glFramebuffer  m_gbuffers[2];
//     idk::glFramebuffer  m_framebuffers[2];

//     GLuint              m_whitenoise;
//     const std::string   m_shader_name = "SVOctree";
//     idk::glSSBO         m_SSBO;


//     idk::SVOctree       voxelize    ( idk::Engine &, int model_id );

//     void                render_quad ( idk::Engine & );
//     void                render      ( idk::Engine &, idk::glFramebuffer );
//     void                place       ( idk::Engine &, int blocktype, int ksize, float span );


// public:
//     void                init        ( idk::Engine & );
//     void                stage_A     ( idk::Engine & );
//     void                stage_B     ( idk::Engine & )   {  };
//     void                stage_C     ( idk::Engine & )   {  };
// };
