#include "Media/Camera.hpp"     // Camera class
#include "Media/Window.hpp"     // Window class
#include "Utility/Assert.hpp"   // GAME_2D_ASSERT macro function
#include "Utility/Math.hpp"     // ConvertToPixels function


// **************
// IMPLEMENTATION
// **************

Camera::Camera():
    m_parent    (nullptr),
    m_transform (0.0f, 0.0f)
{
}



Camera::Camera(const Window& window):
    m_parent    (&window),
    m_transform (0.0f, 0.0f)
{
}



b2Vec2 Camera::GetTransform() const
{
    return m_transform;
}



void Camera::Update(float x, float y)
{
    GAME_2D_ASSERT(m_parent);

    m_transform.x = ConvertToPixels(x) - (m_parent->GetWidth()  / 2.0f);
    m_transform.y = ConvertToPixels(y) + (m_parent->GetHeight() / 2.0f);
}



void Camera::Update(b2Vec2 point)
{
    GAME_2D_ASSERT(m_parent);

    m_transform.x = ConvertToPixels(point.x) - (m_parent->GetWidth()  / 2.0f);
    m_transform.y = ConvertToPixels(point.y) + (m_parent->GetHeight() / 2.0f);
}




void Camera::Reset()
{
    m_transform = b2Vec2(0.0f, 0.0f);
}
