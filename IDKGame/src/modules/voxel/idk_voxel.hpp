#pragma once

#include "IDKengine.h"


class idk_Voxel: idk::Module
{
private:
    idk::SVOctree   m_svo;

public:
    void        init    ( idk::Engine & );
    void        stage_A ( idk::Engine & );
    void        stage_B ( idk::Engine & )    {  };
    void        stage_C ( idk::Engine & )    {  };
};
