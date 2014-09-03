#include "Wall.h"

Wall::Wall(Point position) : GameObject(position, Dimension{20,20}, true, WALL, "wall.png")
{
}

Wall::~Wall()
{
    //dtor
}
