#ifndef WALL_H
#define WALL_H
#include "Coordinate.h""
#include "Tile.h"

class Wall : public Tile
{
    public:
        Wall(Point position);
        virtual ~Wall();
    protected:
    private:
};

#endif // WALL_H
