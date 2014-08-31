#include "Entity.h"

Entity::Entity(Point position, Dimension size) : position_(position), size_(size)
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

Point Entity::getPosition() const   { return position_; }
SDL_Rect Entity::getRect() const    { return SDL_Rect{position_.x, position_.y, size_.x, size_.y};}
