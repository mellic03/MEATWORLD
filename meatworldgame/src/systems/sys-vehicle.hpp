#pragma once

#include <IDKECS/IDKECS.hpp>



namespace meat
{
    class VehicleCmp;
    class VehicleBase;
}


struct meat::VehicleCmp
{
    int   obj_id = -1;
    VehicleBase *actor;

    float steer_speed   = 3.0f;
    float unsteer_speed = 0.1f;
    float max_speed     = 2.0f;
    float acceleration  = 8.0f;
    float decceleration = 0.1f;

    size_t  serialize( std::ofstream &stream ) const { return 0; };
    size_t  deserialize( std::ifstream &stream ) { return 0; };
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id ) {  };
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id ) {  };
    static void onObjectCopy( int src_obj, int dst_obj ) {  };
};

