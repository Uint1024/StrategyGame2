#include "Window.h"

Window::Window(Point position, Dimension size, bool visible, Color color) : position_(position), size_(size), visible_(visible), color_(color)
{
    //ctor
}

Window::~Window()
{
    //dtor
}

void Window::receiveInputs(Inputs& inputs)
{
    if(inputs.getPressedMouseButtons()[SDL_BUTTON_LEFT])
    {
        position_ += inputs.getMouseTravel();
        inputs.setLockedWindow(this);
    }

}

Point Window::getPosition() const {return position_;}
Dimension Window::getSize() const { return size_;}
bool Window::isActive() const { return active_; }
bool Window::isVisible() const { return visible_; }
Color Window::getColor() const { return color_; }
SDL_Rect Window::getRect() const
{
    return SDL_Rect{position_.x, position_.y, size_.x, size_.y};
}
