#pragma once


#include "IDK_builtin_CS_common.h"


class Model_CS: public idk::ComponentSystem
{
private:
    std::vector<int>    _model_ids;
    std::map<int, bool> m_shadowcasts;
    std::vector<GLuint> _shader_ids;
    idk::Engine *engineptr;

public:
    void        init(idk::Engine &);
    void        stage_A(idk::Engine &);
    void        stage_B(idk::Engine &)  {  };
    void        stage_C( idk::Engine & )    {  };

    void        onObjectAssignment( int obj_id, idk::Engine & ) {  };
    void        onObjectCreation( int obj_id, idk::Engine & );
    void        onObjectDeletion( int obj_id, idk::Engine & );
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void        useModel( int obj_id, int model_id, GLuint shader_id );
    idk::Model &getModel( int obj_id );

    void        setShadowcast( int obj_id, bool b ) { m_shadowcasts[obj_id] = b; };

};

