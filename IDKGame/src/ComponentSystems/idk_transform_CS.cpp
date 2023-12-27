#include "idk_transform_CS.h"


void
Transform_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_keys[obj_id] = m_transforms.create();

    std::get<1>(m_transforms.get(m_keys[obj_id])) = -1;
}


void
Transform_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{
    m_keys[obj_id] = -1;
}


void
Transform_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    int &key = m_keys[obj_id];
    m_transforms.destroy(key);
    key = -1;
}


void
Transform_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    engine.giveComponent(dest_obj_id, this->m_id);
}


void 
Transform_CS::translate( int obj_id, glm::vec3 v )
{
    #ifdef IDK_DEBUG
        if (m_keys[obj_id] == -1)
        {
            std::cout << "m_transform_IDs[" << obj_id << "] == -1" << std::endl;
            exit(1);
        }
    #endif

    getTransform(obj_id).translate(v);
}


idk::Transform &
Transform_CS::getTransform( int obj_id )
{
    int key = m_keys[obj_id];
    IDK_ASSERT("Object does not have transform component", key != -1);
 
    return std::get<2>(m_transforms.get(key));
}


void
Transform_CS::rotateX( int obj_id, float r )
{
    getTransform(obj_id).rotateX(r);
}


void
Transform_CS::rotateY( int obj_id, float r )
{
    getTransform(obj_id).rotateY(r);
}


void
Transform_CS::rotateZ( int obj_id, float r )
{
    getTransform(obj_id).rotateZ(r);
}


void 
Transform_CS::scale( int obj_id, glm::vec3 s )
{
    getTransform(obj_id).scale(s);
}



void
Transform_CS::setParent( int parent_id, int child_id )
{
    int key = m_keys[child_id];
    std::get<1>(m_transforms.get(key)) = parent_id;
}


void
Transform_CS::unsetParent( int child_id )
{
    int key = m_keys[child_id];
    std::get<1>(m_transforms.get(key)) = -1;
}


glm::mat4
Transform_CS::getModelMatrix( int obj_id )
{
    glm::mat4 transform = getTransform(obj_id).modelMatrix();

    int key = m_keys[obj_id];
    int parent_id = std::get<1>(m_transforms.get(key));

    while (parent_id != -1)
    {
        transform = getTransform(parent_id).modelMatrix() * transform;
    
        key = m_keys[parent_id];
        parent_id = std::get<1>(m_transforms.get(key));
    }

    return transform;
}
