#include "ui-elements.hpp"
#include "../world.hpp"

#include <libidk/idk_geometry.hpp>
#include <IDKIO/IDKIO.hpp>


meatui::Window::Window( const glm::vec2 &pos, meat::World &world )
:   idkui::Element(window_style),
    m_position(pos)
{
    const auto &callback = [this]( int dx, int dy )
    {
        glm::vec2 mpos = idkio::mousePosition();

        if (idk::geometry::pointInRect(mpos, m_corner, m_span))
        {
            m_position += glm::vec2(dx, dy);
        }
    };

    m_callback_id = idkio::onMouseDrag(idkio::LMOUSE, callback);
}



meatui::Window::~Window()
{
    idkio::removeMouseDragCallback(idkio::LMOUSE, m_callback_id);
}





void
meatui::Window::updateLayout( const glm::vec2 &corner, const glm::vec2 &span, int depth )
{
    m_outer_bounds = computeOuterBounds(m_position, m_size, depth, m_style);
    m_inner_bounds = computeInnerBounds(m_position, m_size, depth, m_style);

    for (auto *child: children)
    {
        child->updateLayout(m_corner, m_span, m_depth);
    }
}


void
meatui::Window::render( idkui::UIRenderer &ren )
{
    ren.renderPrimitive(m_outer_bounds, m_style.bg);
    ren.renderPrimitive(m_inner_bounds, m_style.fg);

}