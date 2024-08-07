#include "game.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKGraphics/terrain/terrain.hpp>
#include <IDKECS/IDKECS.hpp>

#include <libidk/idk_log.hpp>
#include <libidk/idk_random.hpp>

#include "systems/sys-player.hpp"
#include "systems/sys-grabbable.hpp"
#include "systems/sys-building.hpp"
#include "systems/sys-weapon.hpp"
#include "systems/draw.hpp"

#include "ui/ui.hpp"
#include "player/player.hpp"
#include "prefabs/prefabs.hpp"


#include <iostream>



void
MeatWorldGame::registerModules( idk::EngineAPI &api )
{
    using namespace idk;
    using namespace meatworld;

    meatworld::registerPrefabs();

    ECS2::registerSystem<idk::GrabbableSys>();
    ECS2::registerSystem<meatworld::BuildingSys>();
    ECS2::registerSystem<meatworld::WeaponSys>();
    ECS2::registerSystem<meatworld::PlayerSys>();

    ECS2::registerComponent<idk::PlayerSpawnCmp> ("PlayerSpawn",      "Meatworld");
    ECS2::registerComponent<idk::GrabbableCmp>   ("Grabbable",        "Meatworld");
    ECS2::registerComponent<PickupCmp>           ("Pickup",           "Meatworld");
    ECS2::registerComponent<DoorCmp>             ("Door",             "Meatworld");
    ECS2::registerComponent<WallCmp>             ("Wall",             "Meatworld");
    ECS2::registerComponent<WeaponCmp>           ("Weapon",           "Meatworld");
    ECS2::registerComponent<HitSphereCmp>        ("HitSphere",        "Meatworld");
    ECS2::registerComponent<HitBoxCmp>           ("HitBox",           "Meatworld");
    ECS2::registerComponent<CharacterHitBoxCmp>  ("Character HitBox", "Meatworld");
    ECS2::registerComponent<NPCCmp>              ("NPC",              "Meatworld");
    // ECS2::registerComponent<OLPlayerCmp>         ("OLPlayer",    "Meatworld");

    ECS2::getComponentArray<WallCmp>().userCallback = drawComponent<WallCmp>;

    ECS2::onSceneLoad([this]() { onSceneLoad(); });
}



idkui2::LayoutManager *LM;
bool editor_mode = false;
bool module_mode = false;


void
MeatWorldGame::onSceneLoad()
{
    using namespace idk;

    if (player)
    {
        delete player;
    }

    if (editor_mode)
    {
        player = new meatworld::EditorPlayer();
    }

    else
    {
        player = new meatworld::Player();

        auto &array = ECS2::getComponentArray<PlayerSpawnCmp>();
        if (array.size() == 1)
        {
            auto &cmp = *(array.getData().data());

            TransformSys::getLocalPosition(player->m_obj_id) = TransformSys::getWorldPosition(cmp.obj_id);
            auto &tcmp = TransformSys::getTransformCmp(player->m_obj_id);
            tcmp.yaw   = TransformSys::getTransformCmp(cmp.obj_id).yaw;


            ECS2::giveComponent<KinematicCapsuleCmp>(player->m_obj_id);
            auto &capsule = ECS2::getComponent<KinematicCapsuleCmp>(player->m_obj_id);
            capsule.curr_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
            capsule.prev_pos = capsule.curr_pos;
        }
    }
}



void
MeatWorldGame::setup( const std::vector<std::string> &args, idk::EngineAPI &api )
{
    using namespace idk;

    editor_mode = false;

    for (const auto &arg: args)
    {
        if (arg == "--editor")
        {
            editor_mode = true;
        }

        if (arg == "-lm")
        {
            module_mode = true;
        }
    }

    onSceneLoad();


    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();

    ren.pushRenderOverlay("assets/meatworld-white.png", 0.5f, 2.0f, 0.5f);

    events.onDropFile(".idksc", [](const char *filepath)
    {
        idk::ECS2::load(filepath);
    });

    LM = new idkui2::LayoutManager("./assets/fonts/RodettaStamp.ttf", 24);

    createMainMenu(api, LM, &gamedata);
    createMenu(api, LM, &gamedata);
    createSettings(api, LM, &gamedata);
    createMultiplayer(api, LM, &gamedata);
    createSyndromes(api, LM, &gamedata);

    gamedata.gameui.root = gamedata.gameui.mainmenu_root;
}




void
MeatWorldGame::mainloop( idk::EngineAPI &api )
{
    using namespace idk;

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &events = api.getEventSys();

    float dt = engine.deltaTime();


    // if (module_mode == false)
    // {
    //     SDL_SetWindowSize(ren.getWindow(), ren.width(), ren.height());
    // }


    {
        static int face = ren.loadModel("assets/models/npc/lmao-face.idkvi");
        static int body = ren.loadModel("assets/models/npc/lmao-body.idkvi");
        static std::vector<glm::vec3> joints;
        static std::vector<float> dists;

        if (joints.size() == 0)
        {
            for (int i=0; i<8; i++)
            {
                joints.push_back(idk::randvec3(-1.0f, +1.0f));
                dists.push_back(2.0f);
            }
        }

        static glm::vec3 vel = glm::vec3(0.0f);

        vel += idk::randvec3(-1.0f, +1.0f);

        if (glm::distance(joints.front(), joints.back()) > 5.9f)
        {
            vel = 5.0f * glm::normalize(joints.front() - joints.back());
        }
        joints.back() += dt * vel;


        TransformSys::FABRIK(joints, dists);

        for (int i=0; i<joints.size()-1; i++)
        {
            ren.drawSphere(joints[i], 0.25f);
            ren.drawCapsule(joints[i], joints[i+1], 0.125f);

            glm::mat4 TR = glm::inverse(glm::lookAt(joints[i+1], joints[i], glm::vec3(0.0f, 1.0f, 0.0f)));
            glm::mat4 T  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -dists[0]));
            glm::mat4 S  = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, glm::length(joints[i+1] - joints[i])));

            ren.drawModel(body, TR*T*S);
        }


        glm::mat4 TR = glm::inverse(
            glm::lookAt(joints.back(), glm::vec3(ren.getCamera().position), glm::vec3(0.0f, 1.0f, 0.0f))
        );

        ren.drawModel(face, glm::translate(glm::mat4(1.0f), joints.back()));

    }




    idkui::TextManager::text(10, 10) << "MEATWORLD v0.1.0";

    player->update(api);

    if (events.keylog().keyTapped(idk::Keycode::SPACE))
    {
        ren.skipRenderOverlay();
    }

    if (events.keylog().keyTapped(idk::Keycode::ESCAPE))
    {
        bool b = events.mouseCaptured();
        events.mouseCapture(!b);
    }


    if (events.mouseCaptured())
    {
        gamedata.gameui.root->close();
    }

    else
    {
        gamedata.gameui.root->open();
    }

    ren.getCamera().fov = gamedata.settings.cam_fov;

    if (ECS2::getCurrentScene() == "scenes/mainmenu")
    {
        gamedata.gameui.root = gamedata.gameui.mainmenu_root;
    }

    else
    {
        gamedata.gameui.root = gamedata.gameui.ingame_root;
    }


    LM->renderTexture(api, gamedata.gameui.root);

}



void
MeatWorldGame::shutdown()
{
    LOG_INFO() << "MeatWorldGame::shutdown";

}
