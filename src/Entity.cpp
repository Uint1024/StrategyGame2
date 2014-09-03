#include "Entity.h"
#include "Graphics.h"
#include "Config.h"

Entity::Entity(Point position, Dimension size, bool solid, TILE_TYPE type, std::string texture_name) :
                    position_(position), size_(size), solid_(solid), texture_name_(texture_name), type_(type)
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

Point Entity::getPosition() const   { return position_; }
SDL_Rect Entity::getRect(Graphics& graphics_, Config& config_) const
{
    return SDL_Rect{position_.x * config_.getTileSize().x - graphics_.getCamera().getPosition().x,
                    position_.y * config_.getTileSize().y - graphics_.getCamera().getPosition().y,
                    size_.x,
                    size_.y};
}
std::string Entity::getTextureName() const { return texture_name_;}
TILE_TYPE Entity::getType() const { return type_; }
bool Entity::isSolid() const { return solid_; }
