#include "game.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGraphics/UI/idk_ui2.hpp>
#include <IDKECS/IDKECS.hpp>

#include <libidk/idk_log.hpp>

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
            break;
        }
    }

    onSceneLoad();


    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();

    // ren.useSkybox(ren.loadSkybox("assets/cubemaps/skybox8/"));
    ren.pushRenderOverlay("assets/meatworld-white.png", 0.5f, 2.0f, 0.5f);

    events.onDropFile(".idksc", [](const char *filepath)
    {
        idk::ECS2::load(filepath);
    });

    LM = new idkui2::LayoutManager("./assets/fonts/RodettaStamp.ttf", 32);

    createMainMenu(api, LM, &gamedata);
    createMenu(api, LM, &gamedata);
    createSettings(api, LM, &gamedata);
    createMultiplayer(api, LM, &gamedata);
    createSyndromes(api, LM, &gamedata);

    gamedata.gameui.root = gamedata.gameui.mainmenu_root;

    // Heightmap
    // -----------------------------------------------------------------------------------------
    // static const idk::glTextureConfig config = {
    //     .internalformat = GL_SRGB8_ALPHA8,
    //     .format         = GL_RGBA,
    //     .minfilter      = GL_LINEAR,
    //     .magfilter      = GL_LINEAR,
    //     .wrap_s         = GL_CLAMP_TO_BORDER,
    //     .wrap_t         = GL_CLAMP_TO_BORDER,
    //     .datatype       = GL_UNSIGNED_BYTE,
    //     .genmipmap      = GL_FALSE
    // };

    // idk::TextureWrapper wrapper;
    // idk::gltools::loadTexture("assets/heightmaps/sand-dunes.jpg", config, &wrapper);
    // idk::PhysicsSys::bakeHeightmap(wrapper);
    // -----------------------------------------------------------------------------------------

}




void
MeatWorldGame::mainloop( idk::EngineAPI &api )
{
    using namespace idk;

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &events = api.getEventSys();

    float dt = engine.deltaTime();

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
