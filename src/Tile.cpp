#include "Tile.h"

Tile::Tile(Point position,  bool solid, TILE_TYPE type) : Entity(position, Dimension{20,20}), solid_(solid), type_(type)
{
    //ctor
}

Tile::~Tile()
{
    //dtor
}


TILE_TYPE Tile::getType() const { return type_; }
