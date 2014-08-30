#ifndef PLAINS_H
#define PLAINS_H

#include "Tile.h"
#include <SDL.h>
class Plains : public Tile
{
    public:
        Plains(Point position);
        virtual ~Plains();
    protected:
    private:
};

#endif // PLAIN_H
