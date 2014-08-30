#ifndef TILE_H
#define TILE_H
#include "Coordinate.h"
#include <SDL.h>
class Tile
{
    public:
        Tile(Point position, bool solid, TILE_TYPE type);
        virtual ~Tile();
        Point getPosition() const;
        TILE_TYPE getType();
        Uint16 getLevel();
        Uint16 getPriority();
    protected:
        //Dimension size_; // in config.h
        Point position_; //position in tiles, not in pixel, so (10,10) means 10th tile from the top and from the left
        bool solid_; //can't walk through solid tiles
        TILE_TYPE type_;
        Uint16 level_, priority;

};

#endif // TILE_H
