#include "./state.hpp"


static constexpr uint32_t BLAMBO_WAITING_ON_CALLBACK = 0;
static constexpr uint32_t BLAMBO_INITIAL_STATE       = 1;
static constexpr uint32_t BLAMBO_WALKING_TO_SPHERE   = 2;
static constexpr uint32_t BLAMBO_SPEAKING_AT_SPHERE  = 3;
static constexpr uint32_t BLAMBO_WALKING_TO_PLAYER   = 4;
static constexpr uint32_t BLAMBO_SPEAKING_AT_PLAYER  = 5;
static constexpr uint32_t BLAMBO_WALKING_TO_OFFICE   = 6;


void idkg::update_blambo( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    int player = ecs.getGameObjectByName("Player");
    int blambo = ecs.getGameObjectByName("Dr-Blambo");
    int office = ecs.getGameObjectByName("Medical Office");
    int progression_1 = ecs.getGameObjectByName("progression-1");
    int progression_2 = ecs.getGameObjectByName("progression-2");
    int progression_3 = ecs.getGameObjectByName("progression-3");

    int diagnosis_wav = ecs.getGameObjectByName("AIDS-diagnosis.wav");
    int congrats_wav  = ecs.getGameObjectByName("AIDS-congrats.wav");
    int believe_wav   = ecs.getGameObjectByName("cant-believe.wav");


    if (player == -1 || blambo == -1)
    {
        return;
    }

    auto &player_states = ecs.getComponent<idk::ProgressionStateCmp>(player).states;
    auto &blambo_states = ecs.getComponent<idk::ProgressionStateCmp>(blambo).states;

    float dist = idk::TransformSys::getDistanceWorldspace(player, blambo);
    float dist_bl_pr = idk::TransformSys::getDistanceWorldspace(blambo, progression_1);

    if (dist == 0.0f)
    {
        return;
    }


    static int target_obj  = progression_1;
    uint32_t &blambo_state = blambo_states[0];


    idk::TransformSys::lookTowards(blambo, target_obj, 0.5f);

    switch (blambo_state)
    {
        default:
            break;

        case BLAMBO_INITIAL_STATE:

            break;


        case BLAMBO_WALKING_TO_SPHERE:
        {
            target_obj   = progression_1;
            blambo_state = BLAMBO_WAITING_ON_CALLBACK;

            idk::PhysXSys::moveTowardsCallbackCCT(blambo, progression_1, 0.1f, 1.5f, [&]()
            {
                blambo_state = BLAMBO_SPEAKING_AT_SPHERE;
            });
    
            break;
        }


        case BLAMBO_SPEAKING_AT_SPHERE:
        {
            blambo_state = BLAMBO_WAITING_ON_CALLBACK;

            idk::AudioSys::playSoundCallback(diagnosis_wav, [&]()
            {
                blambo_state = BLAMBO_WALKING_TO_PLAYER;
            });

            break;
        }


        case BLAMBO_WALKING_TO_PLAYER:
        {
            target_obj   = player;
            blambo_state = BLAMBO_WAITING_ON_CALLBACK;

            idk::PhysXSys::moveTowardsCallbackCCT(blambo, player, 0.1f, 2.5f, [&]()
            {
                blambo_state = BLAMBO_SPEAKING_AT_PLAYER;
            });

            break;
        }


        case BLAMBO_SPEAKING_AT_PLAYER:
        {
            blambo_state = BLAMBO_WAITING_ON_CALLBACK;

            idk::AudioSys::playSoundCallback(congrats_wav, [&]()
            {
                blambo_state = BLAMBO_WALKING_TO_OFFICE;
            });

            break;
        }


        case BLAMBO_WALKING_TO_OFFICE:
        {
            target_obj   = progression_2;
            blambo_state = BLAMBO_WAITING_ON_CALLBACK;

            idk::PhysXSys::moveTowardsCallbackCCT(blambo, progression_2, 0.1f, 1.5f, [&]()
            {
                blambo_state = 14;
            });

            break;
        }


        case 14:
        {
            target_obj   = progression_3;
            blambo_state = BLAMBO_WAITING_ON_CALLBACK;

            idk::PhysXSys::moveTowardsCallbackCCT(blambo, progression_3, 0.1f, 1.5f, [&]()
            {
                blambo_state = 15;
            });

            break;
        }

    }


}


