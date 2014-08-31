#ifndef ENTITY_H
#define ENTITY_H
#include "Coordinate.h"

class Entity
{
    public:
        Entity(Point position, Dimension size);
        virtual ~Entity();
        Point getPosition() const;
        SDL_Rect getRect() const;
    protected:
        Point position_; //position in tiles, not in pixel, so (10,10) means 10th tile from the top and from the left
        Dimension size_;
    private:
};

#endif // ENTITY_H
