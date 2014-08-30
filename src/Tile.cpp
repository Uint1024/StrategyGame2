#include "Tile.h"

Tile::Tile(Point position,  bool solid, TILE_TYPE type) : position_(position), solid_(solid), type_(type)
{
    //ctor
}

Tile::~Tile()
{
    //dtor
}

Point Tile::getPosition() const { return position_; }
TILE_TYPE Tile::getType() { return type_; }
