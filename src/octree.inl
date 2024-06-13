#pragma once

#include "./octree.hpp"


template <typename T>
void
idkg::Octree<T>::_insert( const glm::vec3 &pos, const glm::vec3 &center,
                          int group, int node, float span, int depth, const T& data )
{
    if (depth >= MAX_DEPTH)
    {
        getNode(group, node).data = data;
    }

    if (getNode(group, node).children == -1)
    {
        giveChildren(group, node);
    }

    int octant   = getOctant(pos, center);
    int children = getNode(group, node).children;

    _insert(pos, center, children, octant, span/2.0f, data);
}


template <typename T>
void
idkg::Octree<T>::insert( const glm::vec3 &pos, const T& data )
{
    _insert(pos, m_center, 0, m_root, m_span, 0, data);
}




template <typename T>
int
idkg::Octree<T>::getOctant( const glm::vec3 &pos, const glm::vec3 &center )
{
    int octant = 0;

    if (pos.x < center.x) octant |= 1;
    if (pos.y < center.y) octant |= 2;
    if (pos.z < center.z) octant |= 4;

    return octant;
}


template <typename T>
void
idkg::Octree<T>::giveChildren( int group, int node )
{
    getNode(group, node).children = m_groups.create();
}


template <typename T>
idkg::Octree<T>::Node&
idkg::Octree<T>::getNode( int group, int node )
{
    return m_groups.get(group)[node];
}


template <typename T>
idkg::Octree<T>::Group&
idkg::Octree<T>::getGroup( int group )
{
    return m_groups.get(group);
}


