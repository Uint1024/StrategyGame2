#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>
#include "Coordinate.h"

class Config;
class Graphics;
class Entity
{
    public:
        Entity(Point position, Dimension size, bool solid, TILE_TYPE type, std::string texture_name);
        virtual ~Entity();
        Point getPosition() const;
        SDL_Rect getRect(Graphics& graphics_, Config& config_) const;
        std::string getTextureName() const;
        TILE_TYPE getType() const;
        bool isSolid() const;
        bool solid_; //can't walk through solid tiles
        TILE_TYPE type_;
    protected:
        Point position_; //position in tiles, not in pixel, so (10,10) means 10th tile from the top and from the left
        Dimension size_;
        std::string texture_name_;
    private:
};

#endif // ENTITY_H
