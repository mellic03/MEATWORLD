#include "terrain_cs.hpp"
#include "../idk_transform_CS.h"
#include "../idk_model_CS.hpp"


int chunked_id = -1;
int CRQ_id     = -1;
int RQ_id      = -1;

void
idkg::Terrain_CS::init( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    const idk::RenderQueueConfig grass_config = {
        .cull_face = false
    };

    const idk::RenderQueueConfig terrain_config = {
        .cull_face = true
    };

    ren.createProgram(
        "GRASS-PROGRAM",
        "IDKGE/shaders/deferred/geometry-pass/",
        "instanced.vs",
        "instanced.fs"
    );

    ren.createProgram(
        "TERRAIN-PROGRAM",
        "IDKGE/shaders/deferred/geometry-pass/",
        "terrain.vs",
        "terrain.fs"
    );

    // RQ_id  = ren.createRenderQueue(
    //     "TERRAIN-PROGRAM", terrain_config, idk::drawmethods::draw_heightmapped
    // );

    CRQ_id = ren.createRenderQueue(
        "GRASS-PROGRAM", grass_config, idk::drawmethods::draw_instanced
    );

}


void
idkg::Terrain_CS::stage_A( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    Transform_CS &tCS = engine.getCS<Transform_CS>();

    // if (chunked_id != -1)
    // {
    //     ren.drawModelRQ(CRQ_id, chunked_id, glm::mat4(1.0f));
    // }

    for (auto &[obj_id, model_id, animator_id]: m_object_model_ids)
    {
        glm::mat4 model_mat = tCS.getModelMatrix(obj_id);

        ren.drawModel(model_id, model_mat);
    }
}



void
idkg::Terrain_CS::useModel( int obj_id, int model_id )
{
    m_keys[obj_id] = m_object_model_ids.create({obj_id, model_id, -1});
}



idk::Model &
idkg::Terrain_CS::getModel( idk::RenderEngine &ren, int obj_id )
{
    int key      = m_keys[obj_id];
    int model_id = m_object_model_ids.get(key)[1];
    return ren.modelSystem().getModel(model_id);
}


void
idkg::Terrain_CS::generateGrass( int obj_id, int terrain_id, GLuint heightmap, int grass_id )
{
    // constexpr int CHUNKS_W   = 32;
    // constexpr int NUM_CHUNKS = (CHUNKS_W * CHUNKS_W);

    // std::vector<glm::vec4>              chunk_positions(NUM_CHUNKS, glm::vec4(0.0f));
    // std::vector<std::vector<glm::mat4>> chunk_transforms(NUM_CHUNKS);

    // auto &MS = engine.RenderEngine().modelSystem();
    // idk::glTexture &texture = MS.getidkTexture(heightmap);

    // glm::ivec2 size = texture.size(4);

    // std::cout << "size: " << size.x << ", " << size.y << "\n";

    // for (int y=0; y<size.y; y++)
    // {
    //     for (int x=0; x<size.x; x++)
    //     {
    //         float height = MS.queryTerrainHeight(terrain_id, glm::mat4(1.0f), x, y);

    //         if (height < 1.5f)
    //         {
    //             int row = y / 8;
    //             int col = x / 8;

    //             glm::vec3 pos = glm::vec3(float(x), 0.0f, float(y));

    //             int idx = CHUNKS_W*row + col;
    //             chunk_positions[idx] += glm::vec4(pos, 0.0f);

    //             glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
    //             chunk_transforms[idx].push_back(T);
    //         }
    //     }
    // }


    // for (int i=0; i<CHUNKS_W; i++)
    // {
    //     for (int j=0; j<CHUNKS_W; j++)
    //     {
    //         if (chunk_transforms[CHUNKS_W*i + j].size() == 0)
    //         {
    //             continue;
    //         }

    //         int idx = CHUNKS_W*i + j;
    //         chunk_positions[idx] /= chunk_transforms[idx].size();
    //         chunk_positions[idx].w = 16.0f;
    //     }
    // }

    // chunked_id = MS.createChunkedModel(grass_id, chunk_positions, chunk_transforms);
}




void
idkg::Terrain_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    m_object_model_ids.destroy(m_keys[obj_id]);
    m_keys[obj_id] = -1;
}



void
idkg::Terrain_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    engine.giveComponent(dest_obj_id, this->m_id);

    int key      = m_keys[src_obj_id];
    int model_id = m_object_model_ids.get(key)[1];

    m_keys[dest_obj_id] = m_object_model_ids.create({dest_obj_id, model_id});
}


