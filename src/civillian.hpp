#pragma once

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKECS/IDKECS.hpp>


namespace idkg
{
    class Civillian;

};



class idkg::Civillian
{
private:
    int   m_ID;
    float heading = 0.0f;

public:

         Civillian( idk::EngineAPI &api, int planet );
    void update( idk::EngineAPI &api, int player );

};



