#include "idk_model_CS.h"
#include "idk_transform_CS.h"


void
Model_CS::init(idk::Engine &engine)
{
    addDependencies("transform");
    engineptr = &engine;
}


void
Model_CS::stage_A(idk::Engine &engine)
{
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");
    idk::RenderEngine &ren = engine.rengine();


    const std::set<int> &obj_ids = engine.gameObjects_byComponent(m_id);

    for (int obj_id: obj_ids)
    {
        glm::mat4 model_mat = tCS.getModelMatrix(obj_id);

        if (m_shadowcasts[obj_id])
        {
            ren.drawShadowCaster(_model_ids[obj_id], model_mat);
        }

        ren.drawModel(_shader_ids[obj_id], _model_ids[obj_id], model_mat);
    }
}

void
Model_CS::useModel( int obj_id, int model_id, GLuint shader_id )
{
    _model_ids[obj_id] = model_id;
    _shader_ids[obj_id] = shader_id;
};

idk::Model &
Model_CS::getModel( int obj_id )
{
    return engineptr->rengine().modelManager().getModel(_model_ids[obj_id]);
}



void
Model_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{
    if (obj_id >= _model_ids.size())
    {
        _model_ids.resize(obj_id+1, -1);
        _shader_ids.resize(obj_id+1, -1);
    }
    else
    {
        _model_ids[obj_id] = -1;
    }
}


void
Model_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    _model_ids[obj_id] = -1;
}



void
Model_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    engine.giveComponent(dest_obj_id, this->m_id);
    _model_ids[dest_obj_id] = _model_ids[src_obj_id];
    _shader_ids[dest_obj_id] = _shader_ids[src_obj_id];
}


