#include "Tree.h"
#include "Peasant.h"
#include <iostream>

Tree::Tree(Point position) : Tile(position, true, TREE, "tree.png"), wood_(4)
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
    npc->addWood(1);
    wood_ -= 1;

    return wood_;
}
