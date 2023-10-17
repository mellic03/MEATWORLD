#include "idk_svoctree.hpp"



idk::SVOctree::SVOctree( float span, float minimum ): SPAN(span), MINIMUM(minimum)
{
    m_root_id = new_node(glm::ivec3(0), true);
}


int
idk::SVOctree::get_octant( glm::vec3 pos, glm::vec3 center )
{
    constexpr int OCTANTS[2][2][2] = {
        {{1, 0}, {5, 4}},
        {{2, 3}, {6, 7}}
    };

    return OCTANTS[pos.y<center.y][pos.z>center.z][pos.x>center.x];
}


glm::vec3
idk::SVOctree::shift_center( int octant, glm::vec3 center, float span )
{
    constexpr glm::vec3 OFFSETS[8] = {
        {+1.0f, +1.0f, -1.0f}, {-1.0f, +1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f}, {+1.0f, -1.0f, -1.0f},

        {+1.0f, +1.0f, +1.0f}, {-1.0f, +1.0f, +1.0f},
        {-1.0f, -1.0f, +1.0f}, {+1.0f, -1.0f, +1.0f}
    };

    return center + (span/4.0f) * OFFSETS[octant];
}

int
idk::SVOctree::new_node( glm::ivec3 color, bool leaf )
{
    int id = m_nodes.create();
    m_nodes.get(id).color = color;
    m_nodes.get(id).leaf  = leaf;

    for (int i=0; i<8; i++)
    {
        m_nodes.get(id).children[i] = -1;
    }

    return id;
}


void
idk::SVOctree::f_insert( int id, glm::ivec3 color, glm::vec3 pos, glm::vec3 center, int depth )
{
    m_nodes.get(id).leaf = false;

    const float span = SPAN / pow(2, depth);
    if (span <= MINIMUM)
    {
        m_nodes.get(id).leaf = true;
        return;
    }

    const int octant = SVOctree::get_octant(pos, center);

    for (int i=0; i<8; i++)
    {
        if (m_nodes.get(id).children[i] == -1)
        {
            m_nodes.get(id).children[i] = new_node(glm::ivec3(0), true);
        }
    }


    const glm::vec3 new_center = SVOctree::shift_center(octant, center, span);
    f_insert(m_nodes.get(id).children[octant], color, pos, new_center, depth+1);
}


void
idk::SVOctree::insert( glm::ivec3 color, glm::vec3 position )
{
    f_insert(m_root_id, color, position, glm::vec3(0.0f), 0);
}


