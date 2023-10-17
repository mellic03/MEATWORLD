#pragma once

#include "IDKengine.h"
#include "idk_svoctree.hpp"

class idk_Voxel: public idk::Module
{
private:
    idk::SVOctree m_svo = idk::SVOctree(32.0f, 1.0f / 4.0f);

public:
    void        init    ( idk::Engine & );
    void        stage_A ( idk::Engine & );
    void        stage_B ( idk::Engine & )    {  };
    void        stage_C ( idk::Engine & )    {  };
};
