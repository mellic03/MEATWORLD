#pragma once

#include <cstdint>
#include <cstddef>

#include <libidk/idk_allocator.hpp>
#include <libidk/idk_glm.hpp>


namespace idkg
{
    template <typename T>
    class Octree;
}


template <typename T>
class idkg::Octree
{
private:
    static constexpr int MAX_DEPTH = 5;

    struct Node
    {
        // T   data;
        int data     = -1;
        int children = -1;
    };

    // struct Leaf
    // {
    //     T data[8][8][8];
    // };

    using Leaf  = T[8][8][8];
    using Group = Node[8];

    int                     m_root;
    idk::Allocator<Leaf>    m_leaves;
    idk::Allocator<Group>   m_groups;

    const glm::vec3         m_center;
    const float             m_span;

    void _insert( const glm::vec3 &pos, const glm::vec3 &center,
                  int group, int node, float span, int depth, const T& data );


    int   getOctant( const glm::vec3 &pos, const glm::vec3 &center );
    void  giveChildren( int group, int node );
    Node  &getNode( int group, int node );
    Group &getGroup( int group );


public:
    Octree( const glm::vec3 &center, float span )
    :   m_center(center),
        m_span(span)
    {
    }

    void insert( const glm::vec3 &position, const T& data );

};


#include "./octree.inl"
