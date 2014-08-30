#include "Wall.h"

Wall::Wall(Point position) : Tile(position, true, WALL)
{
}

Wall::~Wall()
{
    //dtor
}
