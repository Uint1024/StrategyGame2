#ifndef TILE_H
#define TILE_H
#include "Coordinate.h"
#include <SDL.h>
#include "Entity.h"
class GameObject : public Entity
{
    public:
        GameObject(Point position, Dimension size, bool solid, TILE_TYPE type, std::string texture_name);
        virtual ~GameObject();

    protected:



};

#endif // TILE_H
