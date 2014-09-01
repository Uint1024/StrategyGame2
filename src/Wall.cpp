#include "Wall.h"

Wall::Wall(Point position) : Tile(position, true, WALL, "wall.png")
{
}

Wall::~Wall()
{
    //dtor
}
