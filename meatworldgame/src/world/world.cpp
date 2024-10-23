#include "world.hpp"

#include <libidk/idk_log.hpp>
#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKIO/IDKIO.hpp>

#include "../character/character.hpp"
#include "../vehicle/vehicle.hpp"
#include "../character/weapon.hpp"

#include "ui/ingame-ui.hpp"
#include "ui/menu-ui.hpp"
#include "ui/ui-style.hpp"


meat::World::World( idk::EngineAPI &api )
:   apiref(api)
{
    ui.ingame_menu  = meatui::ingame_menu(api, *this);
    ui.ingame_ui    = meatui::ingame_ui(api, *this);
    ui.inventory_ui = meatui::inventory_ui(api, *this);

    ui.crosshair = new idkui::ImageAbsolute(ui.textures[4], meatui::crosshair_style);
    ui.crosshair->setPosition(api.getRenderer().winsize() / 2);

    ui.hitmarker_img = new idkui::ImageAbsolute(ui.textures[5], meatui::crosshair_style);

    ui.settings      = meatui::settings_menu(api, *this);
    ui.onscreen_text = new idkui::TextAbsolute("Retarded", meatui::onscreentext_style);
    ui.menu_ui       = new idkui::Stack(meatui::root_style);



    EM.on("player-attack", [this](meat::ActorBase *A){
        ui.crosshair->expand();
    });

    EM.on("open-inventory", [this](meat::ActorBase *A){
        ui.menu_ui->push(ui.inventory_ui);
    });


    ui.EM.on("menu-open", [this]( idkui::Element *node ) {
        if (ui.menu_ui->top() != node)
        {
            ui.menu_ui->push(node);
        }
    });

    ui.EM.on("menu-close", [this]( idkui::Element *e ) {
        ui.menu_ui->pop();
    });

}


meat::World::~World()
{
    clear();

    delete ui.menu_ui;
}






void
meat::World::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    textPrompt("");

    for (ActorBase *obj: m_objects)
    {
        obj->update(api, *this);
    }

    for (PhysicsBody *body: m_physicsbodies)
    {
        body->update(api.dtime());
        ren.drawRect(body->getTransform());

    }

    auto winsize = ren.winsize();

    ui.menu_ui->updateLayout(glm::vec2(0.0f), winsize, 1);
    ui.ingame_ui->updateLayout(glm::vec2(0.0f), winsize, 1);
    ui.crosshair->updateLayout(glm::vec2(0.0f), winsize, 1);

    {
        auto &cam = ren.getCamera();
        glm::mat4 PV = cam.P * cam.V;
        glm::vec4 proj;

        for (glm::vec3 pos: ui.hitmarker_pos)
        {
            proj   = cam.V * glm::vec4(pos, 1.0f);

            if (proj.z > 0.0f)
            {
                continue;
            }

            proj   = cam.P * proj;
            proj   = (proj / proj.w) * 0.5f + 0.5f;
            proj.x = proj.x * cam.width;
            proj.y = (1.0f - proj.y) * cam.height;

            ui.hitmarker_img->pushPosition(glm::vec2(proj));
        }

        ui.hitmarker_img->updateLayout(glm::vec2(0.0f), winsize, 1);
    }

    ui.onscreen_text->setPosition(winsize.x/2, winsize.y/2);
    ui.onscreen_text->updateLayout(glm::vec2(0.0f), winsize, 1);

    // if (ui.menu_ui->top() == nullptr)
    // {
    //     ui.EM.emit("menu-open", ui.ingame_ui);
    // }

    if (idkio::keyTapped(idk::Keycode::ESCAPE))
    {
        if (ui.menu_ui->top() == nullptr)
        {
            ui.EM.emit("menu-open", ui.ingame_menu);
        }

        else
        {
            ui.EM.emit("menu-close");
        }
    }

    idkio::mouseCapture(!(ui.menu_ui->top() == nullptr));
}





// void
// meat::World::renderText( int x, int y, const std::string &text )
// {
//     ui.onscreen_text->m_label = text;
//     ui.onscreen_text->setPosition(x, y);
// }




void
meat::World::textPrompt( const std::string &text )
{
    ui.onscreen_text->m_label = text;
}


void
meat::World::hitMarker( const glm::vec3 &wpos )
{
    ui.hitmarker_pos.push_back(wpos);

    // auto &cam = apiref.getRenderer().getCamera();
    // auto proj = cam.P * cam.V * glm::vec4(ui.hitmarker_pos, 1.0f);
    //      proj = (proj / proj.w) * 0.5f + 0.5f;

    // ui.hitmarker_img->setPosition(glm::vec2(proj));
}



void
meat::World::clear()
{
    for (int i=0; i<m_objects.size(); i++)
    {
        delete m_objects[i];
    }
    m_objects.clear();
    m_objects_sorted.clear();
}
