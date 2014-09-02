#include "Tree.h"

Tree::Tree(Point position) : Tile(position, true, TREE, "tree.png")
{
    //ctor
}

Tree::~Tree()
{
    //dtor
}


void Tree::sayHi()
{
    std::cout << "the tree says hi" << std::endl;
}
