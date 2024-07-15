#pragma once

#include <IDKECS/IDKECS.hpp>


namespace meatworld
{
    struct TileableFloorCmp;
    class  TileableSys;

}



class meatworld::TileableSys: public idk::ECS2::System
{
private:
    int m_RQ;

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};



struct meatworld::TileableFloorCmp
{
    int obj_id = -1;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};

