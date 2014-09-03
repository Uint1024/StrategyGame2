#include "Tree.h"
#include "Peasant.h"
#include <iostream>

Tree::Tree(Point position) : GameObject(position, Dimension{20,20}, true, TREE, "tree.png"), wood_(10)
{
    //ctor
}

Tree::~Tree()
{
    //dtor
}


//growth and shit
bool Tree::update()
{
    if(wood_ <= 0)
       return false;
    else
        return true;
}

int Tree::mineRessources(Peasant* npc)
{
    npc->addWood(10);
    wood_ -= 10;

    return wood_;
}
