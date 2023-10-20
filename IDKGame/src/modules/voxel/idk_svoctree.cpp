#include "idk_svoctree.hpp"



idk::SVOctree::SVOctree( float span, float minimum ): SPAN(span), MINIMUM(minimum)
{
    m_root_id = new_node(span);
    node(m_root_id).blocktype = 0;
}


int
idk::SVOctree::get_octant( glm::vec3 pos, glm::vec3 center )
{
    int octant = 0;

    if (pos.x < center.x) octant |= 1;
    if (pos.y < center.y) octant |= 2;
    if (pos.z < center.z) octant |= 4;

    return octant;
}


glm::vec3
idk::SVOctree::shift_center( int octant, glm::vec3 center, float span )
{
    glm::vec3 offset;
    
    offset.x = (octant & 1) == 0 ? span/4.0f : -span/4.0f;
    offset.y = (octant & 2) == 0 ? span/4.0f : -span/4.0f;
    offset.z = (octant & 4) == 0 ? span/4.0f : -span/4.0f;

    return center + offset;
}

int
idk::SVOctree::new_node( bool leaf )
{
    int id = m_nodes.create();
    node(id).leaf = leaf;

    for (int i=0; i<8; i++)
    {
        node(id).children[i] = -1;
    }

    return id;
}


void
idk::SVOctree::f_give_children( int id )
{
    for (int i=0; i<8; i++)
    {
        node(id).children[i] = new_node(true);
    }
}


bool
idk::SVOctree::f_children_same( int id )
{
    for (int i=0; i<8; i++)
    {
        int child_id = node(id).children[i];

        if (node(child_id).leaf == false)
        {
            return false;
        }
    }

    for (int i=1; i<8; i++)
    {
        int child1_id = node(id).children[i];
        int child2_id = node(id).children[i-1];

        if (node(child1_id).blocktype != node(child2_id).blocktype)
        {
            return false;
        }
    }

    return true;
}


void
idk::SVOctree::f_insert( int id, int blocktype, glm::vec3 pos, glm::vec3 center, int depth )
{
    const float span = SPAN / pow(2, depth);

    if (span <= MINIMUM)
    {
        node(id).leaf = true;
        node(id).blocktype = blocktype;
        return;
    }

    if (node(id).blocktype == blocktype)
    {
        return;
    }

    if (node(id).leaf == true)
    {
        node(id).leaf = false;
        f_give_children(id);
    }

    const int octant = SVOctree::get_octant(pos, center);
    const glm::vec3 new_center = SVOctree::shift_center(octant, center, span);
    f_insert(node(id).children[octant], blocktype, pos, new_center, depth+1);



    // Merge if all children are the same and are all leaves.
    if (f_children_same(id))
    {
        node(id).leaf = true;
        node(id).blocktype = node(node(id).children[0]).blocktype;

        for (int i=0; i<8; i++)
        {
            int child_id = node(id).children[i];
            node(id).children[i] = -1;
            m_nodes.destroy(child_id);
        }
    }
}


void
idk::SVOctree::insert( int blocktype, glm::vec3 position )
{
    f_insert(m_root_id, blocktype, position, glm::vec3(0.0f), 0);
}


