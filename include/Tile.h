#ifndef TILE_H
#define TILE_H
#include "Coordinate.h"
#include <SDL.h>
#include "Entity.h"
class Tile : public Entity
{
    public:
        Tile(Point position, bool solid, TILE_TYPE type, std::string texture_name);
        virtual ~Tile();
        TILE_TYPE getType() const;
        bool isSolid() const;
    protected:
        bool solid_; //can't walk through solid tiles
        TILE_TYPE type_;


};

#endif // TILE_H
