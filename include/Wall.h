#ifndef WALL_H
#define WALL_H
#include "Coordinate.h""
#include "GameObject.h"

class Wall : public GameObject
{
    public:
        Wall(Point position);
        virtual ~Wall();
    protected:
    private:
};

#endif // WALL_H
