#include "Window.h"
#include "Graphics.h"
Window::Window(Point position, Dimension size, bool visible, Color color) : position_(position), size_(size), visible_(visible), color_(color)
{
    Icon peasant_icon{Point{5,5}, "peasant.png", PEASANT};
    Icon wall_icon{Point{25,5}, "wall.png", WALL};
    icons_list_.push_back(peasant_icon);

    icons_list_.push_back(wall_icon);
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

void Window::render(Graphics& graphics)
{
    graphics.drawWindow(this, icons_list_);
    /*for(auto icon& : icons_list_)
    {
        graphics.renderIcon(icon);
    }*/
}
