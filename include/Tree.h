#ifndef TREE_H
#define TREE_H
#include "Entity.h"
#include "Tile.h"
#include "Coordinate.h"

class Tree : public Tile
{
    public:
        Tree(Point position);
        virtual ~Tree();
        void sayHi();
    protected:

    private:
};

#endif // TREE_H
