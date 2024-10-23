#pragma once

#include "ui-style.hpp"


namespace meat { class World; }

namespace meatui
{
    class Window;
    class Menu;
    class Button;
    class GridSquare;
}



class meatui::Window: public idkui::Element
{
private:
    glm::vec2 m_position = glm::vec2(0.0f);
    glm::vec2 m_size     = glm::vec2(512, 800);
    int m_callback_id;

public:
    Window( const glm::vec2 &pos, meat::World& );
    ~Window();

    void pushChild( idkui::Element *node )
    {
        children.push_back(node);
    }

    virtual void updateLayout( const glm::vec2&, const glm::vec2&, int ) final;
    virtual void render( idkui::UIRenderer& ) final;
};




class meatui::Menu: public idkui::List
{
private:

public:
    Menu( const std::string &title, const std::vector<Element*> &args )
    :   idkui::List(args.size()+1, menu_style)
    {
        children[0] = new idkui::Text(title, menutitle_style);

        for (int i=0; i<args.size(); i++)
        {
           children[1+i] = args[i];
        }
    }
};




class meatui::Button: public idkui::Button
{
private:

public:
    Button( const std::string &title, const std::function<void()> &callback )
    :   idkui::Button(title, button_style, buttontext_style, callback)
    {

    }

};




class meatui::GridSquare: public idkui::Image
{
private:


public:
    GridSquare( uint32_t texture )
    :   idkui::Image(texture, gridsquare_style)
    {

    }

    void render( idkui::UIRenderer &ren )
    {
        ren.renderPrimitive(m_outer_bounds, m_style.fg);
        ren.renderImage(m_inner_bounds, m_texture);
    }

    virtual void onHover() override
    {
        m_style.padding = glm::mix(m_style.padding, glm::vec4(0.0f), 0.1f);
        m_style.margin  = glm::mix(m_style.margin,  glm::vec4(0.0f), 0.1f);
    }

    virtual void offHover() override
    {
        m_style.padding = glm::mix(m_style.padding, m_style2.padding, 0.5f);
        m_style.margin  = glm::mix(m_style.margin,  m_style2.margin,  0.1f);
    }

};
