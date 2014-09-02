#include "Tile.h"

Tile::Tile(Point position,  bool solid, TILE_TYPE type, std::string texture_name) : Entity(position, Dimension{20,20}, texture_name), solid_(solid), type_(type)
{
    //ctor
}

Tile::~Tile()
{
    //dtor
}


TILE_TYPE Tile::getType() const { return type_; }
bool Tile::isSolid() const { return solid_; }
