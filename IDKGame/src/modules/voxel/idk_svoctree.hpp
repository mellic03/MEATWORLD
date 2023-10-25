#pragma once

#include "IDKengine.h"


namespace idk { class SVOctree; };


class idk::SVOctree
{
public:
    struct Node
    {
        int blocktype, children, mode, pad2;
        glm::vec4 irradiance;
    };

    struct Nodes
    {
        Node nodes[8];
    };


private:
    idk::Allocator<Nodes>   m_nodegroups;

    bool                f_children_same( int id );
    void                f_insert( int id, int blocktype, float blockspan, glm::vec3 pos, glm::vec3 center, int depth );


public:

    float               SPAN;
    int                 m_root_id;

                        SVOctree( float span, float minimum );
    void                insert( int blocktype, glm::vec3 position, float blockspan );

    Allocator<Nodes> &  nodegroups()            { return m_nodegroups; };

    int                 nodegroup_new( int blocktype );
    int                 nodegroup_from( int groupid, int octant );
    Nodes &             nodegroup( int id )     { return m_nodegroups.get(id);  };
    Node &              node( int groupid, int octant );

    Node                getnode( glm::vec3 pos, glm::vec3 &center );
    glm::vec3           hitpoint( glm::vec3 pos, glm::vec3 dir );

    static int          get_octant( glm::vec3 center, glm::vec3 position );
    static glm::vec3    shift_center( int octant, glm::vec3 center, float span );
};




