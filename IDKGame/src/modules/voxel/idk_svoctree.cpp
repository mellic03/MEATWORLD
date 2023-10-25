#include "idk_svoctree.hpp"



idk::SVOctree::SVOctree( float span, float minimum ): SPAN(span)
{
    m_root_id = nodegroup_new(0);
    node(m_root_id, 0).blocktype = 0;
    node(m_root_id, 0).children = nodegroup_new(0);
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
idk::SVOctree::nodegroup_new( int blocktype )
{
    int id = m_nodegroups.create();

    for (int i=0; i<8; i++)
    {
        node(id, i).children  = -1;
        node(id, i).blocktype = blocktype;
        node(id, i).mode = blocktype;
        node(id, i).irradiance = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    return id;
}



int
idk::SVOctree::nodegroup_from( int groupid, int octant )
{
    int id = m_nodegroups.create();

    for (int i=0; i<8; i++)
    {
        node(id, i).children  = -1;
        node(id, i).blocktype = node(groupid, octant).blocktype;
        node(id, i).irradiance = node(groupid, octant).irradiance;
    }

    return id;
}



idk::SVOctree::Node &
idk::SVOctree::node( int groupid, int octant )
{
    return nodegroup(groupid).nodes[octant];
}


bool
idk::SVOctree::f_children_same( int children_id )
{
    for (int i=0; i<8; i++)
    {
        if (node(children_id, i).children != -1)
        {
            return false;
        }
    }

    for (int i=0; i<7; i++)
    {
        if (node(children_id, i).blocktype != node(children_id, i+1).blocktype)
        {
            return false;
        }
    }

    return true;
}



int get_mode( idk::SVOctree::Nodes nodes )
{
    int counts[8] = { 0 };
    for (int i=0; i<8; i++)
    {
        counts[nodes.nodes[i].blocktype] += 1;
    }

    int max = 0;
    int mode = 0;

    for (int i=0; i<8; i++)
    {
        if (counts[i] > max)
        {
            max = counts[i];
            mode = i;
        }
    }

    return mode;
}


void
idk::SVOctree::f_insert( int groupid, int blocktype, float blockspan, glm::vec3 pos, glm::vec3 center, int depth )
{
    const float span = SPAN / pow(2, depth);
    const int octant = SVOctree::get_octant(pos, center);

    // if (node(groupid, octant).blocktype == blocktype)
    // {
    //     return;
    // }

    if (span <= blockspan)
    {
        node(groupid, octant).blocktype = blocktype;
        return;
    }

    if (node(groupid, octant).children == -1)
    {
        node(groupid, octant).children = nodegroup_new(node(groupid, octant).blocktype);
    }


    center = SVOctree::shift_center(octant, center, span);

    int children_id = node(groupid, octant).children;
    f_insert(children_id, blocktype, blockspan, pos, center, depth+1);


    // Merge if all children are the same and are all leaves.
    if (f_children_same(children_id))
    {
        node(groupid, octant).blocktype = node(children_id, 0).blocktype;
        node(groupid, octant).children = -1;

        m_nodegroups.destroy(children_id);
    }

    else
    {
        int mode = get_mode(nodegroup(groupid));
        node(groupid, octant).mode = mode;
    }

}


void
idk::SVOctree::insert( int blocktype, glm::vec3 position, float blockspan )
{
    f_insert(node(m_root_id, 0).children, blocktype, blockspan, position, glm::vec3(0.0f), 0);
}



idk::SVOctree::Node
idk::SVOctree::getnode( glm::vec3 pos, glm::vec3 &center )
{
    Node n = node(m_root_id, 0);
    center = glm::vec3(0.0);
    float span = SPAN;

    while (n.children != -1)
    {
        int octant = get_octant(pos, center);
        n = node(n.children, octant);

        center = shift_center(octant, center, span);
        span /= 2.0;
    }

    return n;
}


glm::vec3
idk::SVOctree::hitpoint( glm::vec3 pos, glm::vec3 dir )
{
    dir = glm::normalize(dir);

    for (int i=0; i<50; i++)
    {
        pos += 0.02f*dir;

        glm::vec3 center(0.0);
        Node n = getnode(pos, center);

        if (n.blocktype > 0)
        {
            return pos - 0.01f*dir;
        }
    }

    return glm::vec3(0.0);
}
