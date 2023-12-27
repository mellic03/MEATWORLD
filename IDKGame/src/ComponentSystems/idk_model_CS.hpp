#pragma once

#include <IDKengine/IDKengine.hpp>


class Model_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<std::tuple<int, int, int>>   m_object_model_ids;
    std::map<int, int>      m_keys;
    std::map<int, bool>     m_shadowcasts;
    idk::Engine             *engineptr;

public:
    void            init( idk::Engine & );
    void            stage_A( idk::Engine & );
    void            stage_B( idk::Engine & ) {  };
    void            stage_C( idk::Engine & ) {  };

    void            onObjectAssignment( int obj_id, idk::Engine & ) {  };
    void            onObjectCreation( int obj_id, idk::Engine & )   {  };
    void            onObjectDeletion( int obj_id, idk::Engine & );
    void            onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void            useModel( int obj_id, int model_id );
    idk::Model &    getModel( int obj_id );
    int             getModelID( int obj_id );

    idk::Animator & getAnimator( int obj_id);
    int             getAnimatorID( int obj_id );

    void            setShadowcast( int obj_id, bool b ) { m_shadowcasts[obj_id] = b; };

};

