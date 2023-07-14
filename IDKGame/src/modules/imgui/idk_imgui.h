#pragma once

#include "IDKengine.h"


class ImGui_Module: public idk::Module
{
private:

public:
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & );
    void        stage_B( idk::Engine & ) {  };

};

