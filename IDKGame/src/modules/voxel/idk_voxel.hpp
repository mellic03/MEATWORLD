#pragma once

#include "IDKengine.h"
#include "idk_svoctree.hpp"

class idk_Voxel: public idk::Module
{
private:
    idk::SVOctree       m_svo = idk::SVOctree(32.0f, 1.0f);
    idk::glFramebuffer  m_framebuffer;
    idk::glFramebuffer  m_framebuffer_2;
    idk::glFramebuffer  m_gbuffer;

    GLuint              m_whitenoise;
    idk::glShader       m_shader;
    idk::glSSBO         m_SSBO;

    void        render  ( idk::Engine &, idk::glFramebuffer );
    void        place   ( idk::Engine &, int blocktype, int ksize, float span );

public:
    void        init    ( idk::Engine & );
    void        stage_A ( idk::Engine & );
    void        stage_B ( idk::Engine & )   {  };
    void        stage_C ( idk::Engine & )   {  };
};
