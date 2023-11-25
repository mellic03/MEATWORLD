#include "idk_transform_CS.h"


void
Transform_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_transform_IDs[obj_id] = m_transforms.create();
}


void
Transform_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{
    if (obj_id >= m_transform_IDs.size())
    {
        m_transform_IDs.resize(obj_id+1, -1);
        m_parent_IDs.resize(obj_id+1, -1);
    }

    else
    {
        getTransform(obj_id) = idk::Transform();
        m_transform_IDs[obj_id] = -1;
        m_parent_IDs[obj_id]    = -1;
    }
}


void
Transform_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    m_transforms.destroy(m_transform_IDs[obj_id]);
    m_transform_IDs[obj_id] = -1;
    m_parent_IDs[obj_id]    = -1;
}


void
Transform_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    m_transform_IDs[dest_obj_id] = m_transforms.create();
}


void 
Transform_CS::translate( int obj_id, glm::vec3 v )
{
    #ifdef IDK_DEBUG
        if (m_transform_IDs[obj_id] == -1)
        {
            std::cout << "m_transform_IDs[" << obj_id << "] == -1" << std::endl;
            exit(1);
        }
    #endif

    m_transforms.get(m_transform_IDs[obj_id]).translate(v);
}


void
Transform_CS::rotateX( int obj_id, float r )
{
    m_transforms.get(m_transform_IDs[obj_id]).rotateX(r);
}


void
Transform_CS::rotateY( int obj_id, float r )
{
    m_transforms.get(m_transform_IDs[obj_id]).rotateY(r);
}


void
Transform_CS::rotateZ( int obj_id, float r )
{
    m_transforms.get(m_transform_IDs[obj_id]).rotateZ(r);
}


void 
Transform_CS::scale( int obj_id, glm::vec3 s )
{
    #ifdef IDK_DEBUG
        if (m_transform_IDs[obj_id] == -1)
        {
            std::cout << "m_transform_IDs[" << obj_id << "] == -1" << std::endl;
            exit(1);
        }
    #endif

    m_transforms.get(m_transform_IDs[obj_id]).scale(s);
}



void
Transform_CS::setParent( int parent_id, int child_id )
{
    m_parent_IDs[child_id] = parent_id;
}


void
Transform_CS::unsetParent( int child_id )
{
    m_parent_IDs[child_id] = -1;
}


glm::mat4
Transform_CS::getModelMatrix( int obj_id )
{
    glm::mat4 transform = getTransform(obj_id).modelMatrix();
    int parent_id = m_parent_IDs[obj_id];

    while (parent_id != -1)
    {
        transform = getTransform(parent_id).modelMatrix() * transform;
        parent_id = m_parent_IDs[parent_id];
    }

    return transform;
}
