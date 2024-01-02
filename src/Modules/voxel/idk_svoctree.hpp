#pragma once

#include <IDKGameEngine/IDKengine.hpp>


namespace idk { class SVOctree; };


class idk::SVOctree
{
public:
    struct Node
    {
        int blocktype, children, mode, pad2;
    };

    struct Nodes
    {
        Node nodes[8];
    };


private:
    idk::Allocator<Nodes>   m_nodegroups;
    std::set<int>           m_changes;

    bool                f_children_same( int id );
    void                f_insert( int id, int blocktype, float blockspan, glm::vec3 pos, glm::vec3 center, int depth );
    void                f_clear( int groupid, int octant );

public:

    float               SPAN;
    int                 m_root_id;

                        SVOctree( float span, float minimum );
    void                insert( int blocktype, glm::vec3 position, float blockspan );

    Allocator<Nodes> &  nodegroups()            { return m_nodegroups; };

    int                 nodegroup_new( int blocktype );
    void                nodegroup_destroy( int groupid, int octant );
    int                 nodegroup_from( int groupid, int octant );
    Nodes &             nodegroup( int id )     { return m_nodegroups.get(id);  };
    Node &              node( int groupid, int octant );

    std::set<int> &     getChanges()   { return m_changes;  };
    void                clearChanges() { m_changes.clear(); };

    Node                getnode( glm::vec3 pos, glm::vec3 &center );
    glm::vec3           hitpoint( glm::vec3 pos, glm::vec3 dir );

    static int          get_octant( glm::vec3 center, glm::vec3 position );
    static glm::vec3    shift_center( int octant, glm::vec3 center, float span );
};




