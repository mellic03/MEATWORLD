#include "idk_model_CS.hpp"
#include "idk_transform_CS.h"


void
Model_CS::init(idk::Engine &engine)
{
    addDependencies("transform");
}


void
Model_CS::stage_A(idk::Engine &engine)
{
    Transform_CS &tCS = engine.getCS<Transform_CS>();
    idk::RenderEngine &ren = idk::RenderEngine::get();

    for (auto &[obj_id, model_id, animator_id]: m_object_model_ids)
    {
        glm::mat4 transform = tCS.getModelMatrix(obj_id);
        ren.drawModel(model_id, animator_id, transform);

        if (m_shadowcasts[obj_id])
        {
            ren.drawShadowCaster(model_id, animator_id, transform);
        }
    }

}


void
Model_CS::useModel( int obj_id, int model_id )
{
    int animator_id = idk::RenderEngine::get().modelSystem().getModel(model_id).animator_id;

    m_keys[obj_id] = m_object_model_ids.create({obj_id, model_id, animator_id});
}


int
Model_CS::getModelID( int obj_id )
{
    int key      = m_keys[obj_id];
    int model_id = std::get<1>(m_object_model_ids.get(key));

    return model_id;
}


idk::Model &
Model_CS::getModel( int obj_id )
{
    int model_id = getModelID(obj_id);
    return idk::RenderEngine::get().modelSystem().getModel(model_id);
}


int
Model_CS::getAnimatorID( int obj_id )
{
    int model_id = getModelID(obj_id);
    return idk::RenderEngine::get().modelSystem().getAnimatorID(model_id);
}


idk::Animator &
Model_CS::getAnimator( int obj_id)
{
    int animator_id = getAnimatorID(obj_id);
    return idk::RenderEngine::get().modelSystem().getAnimator(animator_id);
}



void
Model_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    std::cout << "Deleting object " << obj_id << " with Model_CS component\n";

    int &key = m_keys[obj_id];
    m_object_model_ids.destroy(key);
    key = -1;
}



void
Model_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    engine.giveComponent(dest_obj_id, this->m_id);

    int key      = m_keys[src_obj_id];
    int model_id = std::get<1>(m_object_model_ids.get(key));

    m_keys[dest_obj_id] = m_object_model_ids.create({dest_obj_id, model_id, -1});
}


