#ifndef TREE_H
#define TREE_H
#include "Entity.h"
#include "GameObject.h"
#include "Coordinate.h"
class Peasant;
class Tree : public GameObject
{
    public:
        Tree(Point position);
        virtual ~Tree();
        void sayHi();
        int mineRessources(Peasant* npc);
        bool update();
    protected:
        int wood_;
    private:
};

#endif // TREE_H
