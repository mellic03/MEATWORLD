// #pragma once

// #include <IDKGameEngine/IDKengine.hpp>


// namespace idkg { class Terrain_CS; };

// class idkg::Terrain_CS: public idk::ComponentSystem
// {
// private:
//     idk::Allocator<std::array<int, 3>>   m_object_model_ids;
//     std::map<int, int>                   m_keys;

// public:
//     void            init( idk::EngineAPI & );
//     void            stage_A( idk::EngineAPI & );
//     void            stage_B( idk::EngineAPI & ) {  };
//     void            stage_C( idk::EngineAPI & ) {  };

//     void            onObjectAssignment( int obj_id, idk::Engine & ) {  };
//     void            onObjectDeassignment ( int obj_id, idk::Engine & ) {  };
//     void            onObjectCreation( int obj_id, idk::Engine & )   {  };
//     void            onObjectDeletion( int obj_id, idk::Engine & );
//     void            onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

//     void            useModel( int obj_id, int model_id );
//     void            generateGrass( int obj_id, int terrain_id, GLuint heightmap, int grass_model_id );

//     idk::Model &    getModel( idk::RenderEngine &, int obj_id );

// };

