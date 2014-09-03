#include "Window.h"
#include "Graphics.h"
Window::Window(Point position, Dimension size, bool visible, Color color) : position_(position), size_(size), visible_(visible), color_(color)
{
    icons_list_.push_back(Icon{position_ + Point{5,5}, "peasant.png", PEASANT});
    icons_list_.push_back(Icon{position_ + Point{25,5}, "wood_wall_blueprint.png", WOOD_WALL_BLUEPRINT});
    icons_list_.push_back(Icon{position_ + Point{45,5}, "plains.png", PLAINS});
    icons_list_.push_back(Icon{position_ + Point{65,5}, "tree.png", TREE});
    icons_list_.push_back(Icon{position_ + Point{85,5}, "stockpile.png", STOCKPILE});
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
        moveComponents(inputs.getMouseTravel());
        inputs.setLockedWindow(this);
    }

}

void Window::moveComponents(Point movement)
{
    for(auto &icon : icons_list_)
    {
        icon.position_ += movement;
    }
}

TILE_TYPE Window::clickedOnIcon(Point mouse_pos)
{
    TILE_TYPE chosen_icon = EMPTY;
    for(auto &icon : icons_list_)
    {
        if(mouse_pos.intersect(icon.getRect()))
        {
            selected_icon_ = &icon;
           chosen_icon = icon.type_;

        }
    }

    return chosen_icon;
}

Icon* Window::getSelectedIcon() const {return selected_icon_;}
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
