#pragma once

#include "IDKengine.h"


namespace idk { class SVOctree; };


class idk::SVOctree
{
public:
    struct Node
    {
        bool leaf;
        glm::ivec3 color;
        int  children[8];
    };


private:
    idk::Allocator<Node>    m_nodes;


    void                f_insert( int id, glm::ivec3 color, glm::vec3 pos, glm::vec3 center, int depth );


public:

    float                   SPAN;
    float                   MINIMUM;

    int                 m_root_id;
                        SVOctree( float span, float minimum );
    void                insert( glm::ivec3 color, glm::vec3 position );
    auto &              nodes() { return m_nodes; };
    int                 new_node( glm::ivec3 color, bool leaf );

    static int          get_octant( glm::vec3 center, glm::vec3 position );
    static glm::vec3    shift_center( int octant, glm::vec3 center, float span );
};




