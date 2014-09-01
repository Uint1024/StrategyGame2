#include "Icon.h"


Icon::~Icon()
{
    //dtor
}


Icon::Icon(Point position, std::string texture_name, TILE_TYPE type) : position_(position),
                                                                        texture_name_(texture_name),
                                                                        type_(type),
                                                                        size_(Point{20,20})
{

}

Point Icon::getPosition() const { return position_; };
SDL_Rect Icon::getRect() const {return SDL_Rect{position_.x, position_.y, size_.x, size_.y}; };
std::string Icon::getTextureName() const { return texture_name_; };
Point Icon::getSize() const {return size_;}
