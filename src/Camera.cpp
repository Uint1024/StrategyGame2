#include "Camera.h"

Camera::Camera() : position_ (Point{0,0})
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

void Camera::moveCamera(const IVector& movement)
{
    position_ += movement;
    std::cout << "moving camera by" << movement.x << std::endl;
}

Point Camera::getPosition() const { return position_; }
