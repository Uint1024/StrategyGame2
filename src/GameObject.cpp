#include "GameObject.h"

GameObject::GameObject(Point position, Dimension size, bool solid,
                       TILE_TYPE type, std::string texture_name) :
                            Entity( position, size, solid,
                                    type, texture_name)
{
    //ctor
}

GameObject::~GameObject()
{
    //dtor
}



