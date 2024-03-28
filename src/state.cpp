#include "state.hpp"

#include <libidk/idk_random.hpp>


static constexpr uint32_t PLAYER_INITIAL_STATE      = 0;
static constexpr uint32_t PLAYER_ARRIVED_AT_MEDICAL = 1;
static constexpr uint32_t PLAYER_FINISHED_MEDICAL   = 2;
static constexpr uint32_t PLAYER_RETURNED_HOME      = 3;

static constexpr uint32_t MOON_REGION_MEDICAL_OFFICE = 2;


static void
update_player_moon( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    int player = ecs.getGameObjectByName("Player");
    int blambo = ecs.getGameObjectByName("Dr-Blambo");
    int office = ecs.getGameObjectByName("Medical Office");

    if (player == -1)
    {
        return;
    }

    auto     &player_states   = ecs.getComponent<idk::ProgressionStateCmp>(player).states;
    uint32_t &player_progress = player_states[0];
    uint32_t &player_region   = player_states[2];

    auto     &blambo_states = ecs.getComponent<idk::ProgressionStateCmp>(blambo).states;
    uint32_t &blambo_active = blambo_states[0];

    float dist_PL_OF = idk::TransformSys::getDistanceWorldspace(player, office);
    float dist_PL_BL = idk::TransformSys::getDistanceWorldspace(player, blambo);

    if (dist_PL_OF < 16.0f)
    {
        player_region = MOON_REGION_MEDICAL_OFFICE;
    }


}



void idkg::update_player( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    int player = ecs.getGameObjectByName("Player");

    if (player == -1)
    {
        return;
    }


    int sun    = ecs.getGameObjectByName("Sun");
    int downus = ecs.getGameObjectByName("Downus");
    int moon   = ecs.getGameObjectByName("Moon");



    auto &player_states = ecs.getComponent<idk::ProgressionStateCmp>(player).states;

    uint32_t &player_progress = player_states[GameState_PROGRESSION];
    uint32_t &player_planet   = player_states[GameState_CURRENT_PLANET];
    uint32_t &player_region   = player_states[GameState_CURRENT_REGION];



    player_planet = idk::PlanetSys::nearestPlanet(idk::TransformSys::getPositionWorldspace(player));


    if (player_planet == moon)
    {
        idkg::update_blambo(api);
        update_player_moon(api);
    }

    idkg::pew(api);

}


void
idkg::pew( idk::EngineAPI &api )
{
    // static std::deque<int> bullets;
    // static std::deque<glm::vec3> velocities;

    // auto &ecs = api.getECS();
    // int player = ecs.getGameObjectByName("Player");
    // glm::vec3 player_pos = idk::TransformSys::getPositionWorldspace(player);

    // glm::vec3 origin = glm::vec3(300.0f, 60.0f, -2000.0f);

    // static bool first = true;
    // static int roundboye = ecs.createGameObject("Round Boye");

    // if (first)
    // {
    //     ecs.giveComponent<idk::TransformCmp>(roundboye);
    //     ecs.giveComponent<idk::ModelCmp>(roundboye);
    //     ecs.giveComponent<idk::PointlightCmp>(roundboye);


    //     ecs.getComponent<idk::PointlightCmp>(roundboye).diffuse = glm::vec4(2.0f, 0.25f, 0.25f, 1.0f);
    //     ecs.getComponent<idk::PointlightCmp>(roundboye).radius  = 32.0f;

    //     idk::TransformSys::getData(roundboye).position = origin;
    //     idk::ModelSys::assignModel(roundboye, "assets/models/round-boye.idkvi");

    //     first = false;
    // }


    // origin = idk::TransformSys::getPositionWorldspace(roundboye);

    // glm::vec3 dir    = glm::normalize(player_pos - origin);
    // glm::vec3 right  = glm::normalize(dir + idk::TransformSys::getSurfaceRight(player));


    // static float A = 0.0f;
    // static float B = 0.0f;

    // A += 0.0005f;
    // B += 0.0005f;

    // float x = 300.0f + 120.0f * sin(A);
    // float y = 0.0f + 120.0f * sin(A);
    // float z = -2000.0f + 120.0f * cos(B);

    // idk::TransformSys::setPositionWorldspace(roundboye, glm::vec3(x, y, z));




    // static glm::mat4 R = glm::rotate(glm::mat4(1.0f), 0.11f, glm::vec3(0.0f, 1.0f, 0.0f));

    // static int count1 = 0;
    // count1 += 1;


    // if (count1 > 4)
    // {
    //     count1 = 0;

    //     if (bullets.size() < 150)
    //     {
    //         int id = ecs.createGameObject("Bullet");
    //         ecs.giveComponent<idk::TransformCmp>(id);
    //         ecs.giveComponent<idk::ModelCmp>(id);
    //         ecs.giveComponent<idk::PointlightCmp>(id);

    //         idk::TransformSys::getData(id).position = origin;
    //         idk::TransformSys::getData(id).scale    = 0.5f;
    //         idk::ModelSys::assignModel(id, "assets/models/unit-sphere.idkvi");

    //         ecs.getComponent<idk::PointlightCmp>(id).diffuse = glm::vec4(idk::randf(), idk::randf(), idk::randf(), 1.0f);
    //         ecs.getComponent<idk::PointlightCmp>(id).radius  = 8.0f;

    //         bullets.push_back(id);


    //         glm::vec3 jitter = 0.15f * (glm::vec3(idk::randf(), idk::randf(), idk::randf()) * 0.5f - 0.5f);
    //         glm::vec3 velocity = glm::mix(dir, right, 0.0f) + jitter;

    //         velocities.push_back(glm::vec3(0.2f) * velocity);

    //         R = R * glm::rotate(glm::mat4(1.0f), 0.11f, glm::vec3(0.0f, 1.0f, 0.0f));;
    //     }

    //     else
    //     {
    //         ecs.removeComponent<idk::PointlightCmp>(bullets.front());
    //         ecs.removeComponent<idk::ModelCmp>(bullets.front());
    //         ecs.removeComponent<idk::TransformCmp>(bullets.front());
    //         ecs.deleteGameObject(bullets.front());

    //         bullets.pop_front();
    //         velocities.pop_front();
    //     }

    // }



    // for (int i=0; i<bullets.size(); i++)
    // {
    //     auto &pos = idk::TransformSys::getData(bullets[i]).position;
    //     if (pos != pos)
    //     {
    //         idk::TransformSys::getData(bullets[i]).position = origin;
    //     }

    //     idk::TransformSys::translateWorldspace(bullets[i], velocities[i]);
    // }

}

