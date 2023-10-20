#pragma once

#include "IDKengine.h"


namespace idk { class SVOctree; };


class idk::SVOctree
{
public:
    struct Node
    {
        int leaf, blocktype, m_children, pad2;
        int  children[8];
    };

    struct Nodes
    {
        Node nodes[8];
    };


private:
    idk::Allocator<Node>    m_nodes;
    idk::Allocator<Nodes>   m_nodess;

    void                f_give_children( int id );
    bool                f_children_same( int id );
    void                f_insert( int id, int blocktype, glm::vec3 pos, glm::vec3 center, int depth );


public:

    float               SPAN;
    float               MINIMUM;
    int                 m_root_id;

                        SVOctree( float span, float minimum );
    void                insert( int blocktype, glm::vec3 position );
    auto &              nodes()                { return m_nodes;          };
    Node &              node( int id )         { return m_nodes.get(id);  };
    Nodes &             children( int id )     { return m_nodess.get(id); };
    int                 new_node( bool leaf );

    static int          get_octant( glm::vec3 center, glm::vec3 position );
    static glm::vec3    shift_center( int octant, glm::vec3 center, float span );
};




