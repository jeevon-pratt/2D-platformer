#include "Media/Camera.hpp"     // Camera class
#include "Utility/Math.hpp"     // ConvertToPixels function


// **************
// IMPLEMENTATION
// **************

Camera::Camera():
    m_transform (0.0f, 0.0f)
{
}



b2Vec2 Camera::GetTransform() const
{
    return m_transform;
}



void Camera::Update(b2Vec2 focusPoint, uint16_t viewWidth, uint16_t viewHeight)
{
    m_transform.x = ConvertToPixels(focusPoint.x) - (viewWidth / 2.0f);
    m_transform.y = ConvertToPixels(focusPoint.y) + (viewHeight / 2.0f);
}



void Camera::Reset()
{
    m_transform = b2Vec2(0.0f, 0.0f);
}
