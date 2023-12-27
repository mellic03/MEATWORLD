#include "terrain_cs.hpp"
#include "../idk_transform_CS.h"
#include "../idk_model_CS.hpp"


constexpr int CHUNKS_W   = 32;
constexpr int NUM_CHUNKS = (CHUNKS_W * CHUNKS_W);

std::vector<idk::OBB>               chunk_OBBs(NUM_CHUNKS);
std::vector<glm::vec4>              chunk_positions(NUM_CHUNKS, glm::vec4(0.0f));
std::vector<std::vector<glm::mat4>> chunk_transforms(NUM_CHUNKS);

int chunked_id = -1;
int CRQ_id     = -1;
int RQ_id      = -1;

void
idkg::Terrain_CS::init( idk::Engine &engine )
{
    engineptr = &engine;


    const idk::RenderQueueConfig grass_config = {
        .cull_face = false
    };

    const idk::RenderQueueConfig terrain_config = {
        .cull_face = true
    };

    engine.rengine().createProgram(
        "GRASS-PROGRAM",
        "IDKGE/shaders/deferred/",
        "geometrypass-instanced.vs",
        "geometrypass-instanced.fs"
    );

    engine.rengine().createProgram(
        "TERRAIN-PROGRAM",
        "IDKGE/shaders/deferred/",
        "geometrypass-terrain.vs",
        "geometrypass-terrain.fs"
    );

    RQ_id  = engine.rengine().createRenderQueue("TERRAIN-PROGRAM", terrain_config);
    CRQ_id = engine.rengine().createRenderQueue("GRASS-PROGRAM", grass_config);

}


void
idkg::Terrain_CS::stage_A( idk::Engine &engine )
{
    Transform_CS &tCS = engine.getCS<Transform_CS>();
    idk::RenderEngine &ren = engine.rengine();

    ren.drawModelRQ(CRQ_id, chunked_id, glm::mat4(1.0f));

    for (auto &[obj_id, model_id, animator_id]: m_object_model_ids)
    {
        glm::mat4 model_mat = tCS.getModelMatrix(obj_id);

        ren.drawModelRQ(RQ_id, model_id, model_mat);
    }
}



void
idkg::Terrain_CS::useModel( int obj_id, int model_id )
{
    m_keys[obj_id] = m_object_model_ids.create({obj_id, model_id, -1});
}



static void
generate_positions( float step,
                  const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2,
                  const glm::vec3 &n0, const glm::vec3 &n1, const glm::vec3 &n2,
                  std::vector<glm::vec3> &positions )
{
    positions.clear();

    float xmin = glm::min(p0.x, glm::min(p1.x, p2.x));
    float xmax = glm::max(p0.x, glm::max(p1.x, p2.x));
    float zmin = glm::min(p0.z, glm::min(p1.z, p2.z));
    float zmax = glm::max(p0.z, glm::max(p1.z, p2.z));

    for (int i=0; i<150; i++)
    {
        float x = xmin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(xmax - xmin)));
        float z = zmin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(zmax - zmin)));


        if (glm::distance(glm::vec2(x, z), glm::vec2(0.0f)) > 50.0f)
        {
            continue;
        }

        glm::vec3 weights = idk::calculate_barycentric(
            x, z,
            glm::vec2(p0.x, p0.z),
            glm::vec2(p1.x, p1.z),
            glm::vec2(p2.x, p2.z)
        );

        if (weights[0] < 0.0f || weights[1] < 0.0f || weights[2] < 0.0f)
        {
            continue;
        }
    
        glm::vec3 N = n0*weights[0] + n1*weights[1] + n2*weights[2];

        if (N.y < 0.96f)
        {
            continue;
        }
    

        float y = p0.y*weights[0] + p1.y*weights[1] + p2.y*weights[2];
        positions.push_back(glm::vec3(x, y, z));
    }

}



void
idkg::Terrain_CS::generateGrass( int obj_id, int terrain_model_id, int grass_model_id )
{
    auto &MS = engineptr->rengine().modelSystem();
    idk::Model &terrain_model = MS.getModel(terrain_model_id);

    std::vector<glm::vec2> minmax_x(NUM_CHUNKS);
    std::vector<glm::vec2> minmax_y(NUM_CHUNKS);
    std::vector<glm::vec2> minmax_z(NUM_CHUNKS);

    for (int i=0; i<NUM_CHUNKS; i++)
    {
        minmax_x[i][0] = +1000.0f;
        minmax_x[i][1] = -1000.0f;

        minmax_y[i][0] = +1000.0f;
        minmax_y[i][1] = -1000.0f;

        minmax_z[i][0] = +1000.0f;
        minmax_z[i][1] = -1000.0f;
    }

    std::vector<glm::vec3> grass_positions;

    for (size_t i=0; i<terrain_model.m_indices.size(); i+=3)
    {
        idk::Vertex *ptr = reinterpret_cast<idk::Vertex *>(terrain_model.m_vertices->data());

        int idx0 = terrain_model.m_indices[i+0];
        int idx1 = terrain_model.m_indices[i+1];
        int idx2 = terrain_model.m_indices[i+2];

        const idk::Vertex &v0 = ptr[idx0];
        const idk::Vertex &v1 = ptr[idx1];
        const idk::Vertex &v2 = ptr[idx2];


        generate_positions(
            0.2f,
            v0.position, v1.position, v2.position,
            v0.normal, v1.normal, v2.normal,
            grass_positions
        );

        for (auto &p: grass_positions)
        {
            constexpr float SPAN = 1.0f;

            int row = int((p.x + 100.0f) / 12.0f);
            int col = int((p.z + 100.0f) / 12.0f);

            float height = glm::clamp(float(rand() % 400) / 100.0f, 0.8f, 1.2f);
                  height = height * glm::clamp((sin(p.x) + sin(p.z)) / 2.0f + 1.0f, 0.8f, 1.2f);

            float rotation = 0.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(2.0f*M_PI - 0.0f)));

            glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, height, 1.0f));
            glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 T = glm::translate(glm::mat4(1.0f), p);

            chunk_transforms[CHUNKS_W*row + col].push_back(T * R * S);
            chunk_positions[CHUNKS_W*row + col] += glm::vec4(p, 0.0f);

            minmax_x[CHUNKS_W*row + col][0] = glm::min(minmax_x[CHUNKS_W*row + col][0], p.x);
            minmax_x[CHUNKS_W*row + col][1] = glm::max(minmax_x[CHUNKS_W*row + col][1], p.x);

            minmax_y[CHUNKS_W*row + col][0] = glm::min(minmax_y[CHUNKS_W*row + col][0], p.y);
            minmax_y[CHUNKS_W*row + col][1] = glm::max(minmax_y[CHUNKS_W*row + col][1], p.y);

            minmax_z[CHUNKS_W*row + col][0] = glm::min(minmax_z[CHUNKS_W*row + col][0], p.z);
            minmax_z[CHUNKS_W*row + col][1] = glm::max(minmax_z[CHUNKS_W*row + col][1], p.z);
        }
    }


    std::vector<glm::vec4> final_chunk_positions;

    for (int i=0; i<CHUNKS_W; i++)
    {
        for (int j=0; j<CHUNKS_W; j++)
        {
            if (chunk_transforms[CHUNKS_W*i + j].size() == 0)
            {
                continue;
            }

            chunk_OBBs[CHUNKS_W*i + j] = idk::OBB(
                minmax_x[CHUNKS_W*i + j],
                minmax_y[CHUNKS_W*i + j],
                minmax_z[CHUNKS_W*i + j]
            );

            chunk_positions[CHUNKS_W*i + j] /= chunk_transforms[CHUNKS_W*i + j].size();
            chunk_positions[CHUNKS_W*i + j].w = 16.0f;

            final_chunk_positions.push_back(chunk_positions[CHUNKS_W*i + j]);
        }
    }

    chunked_id = MS.createChunkedModel(grass_model_id, final_chunk_positions, chunk_OBBs, chunk_transforms);
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


