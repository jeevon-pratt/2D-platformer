#include "Media/Window.hpp"         // Window class
#include "UI/Widget.hpp"            // Widget class
#include "Utility/Assert.hpp"       // GAME_2D_ASSERT macro function


// **************
// IMPLEMENTATION
// **************

Widget::Widget():
    m_parent (nullptr)
{
}



Widget::Widget(const Window& window):
    m_parent (&window)
{
}



void Widget::SetLabel(const std::string& label)
{
    m_label = label;
}



void Widget::SetParentWindow(const Window& window)
{
    m_parent = &window;
}



void Widget::SetPosition(float normX, float normY)
{
    m_coord.Set(normX, normY);
}



b2Vec2 Widget::GetPosition() const
{
    GAME_2D_ASSERT(m_parent);

    float width  = GetWidth();
    float height = GetHeight();

    float winWidth  = m_parent->GetWidth();
    float winHeight = m_parent->GetHeight();

    b2Vec2 position;
    position.x = (m_coord.x * winWidth) - (width / 2.0f);
    position.y = (m_coord.y * winHeight) - (height / 2.0f);

    return position;
}