#ifndef ICON_H
#define ICON_H
#include "Coordinate.h"
#include <SDL.h>
#include <iostream>

class Icon
{
    public:
        Icon(Point position, std::string texture_name, TILE_TYPE type);
        virtual ~Icon();
        Point getPosition() const;
        SDL_Rect getRect() const;
        std::string getTextureName() const;
        Point getSize() const;
    protected:
        Point position_; //position relative to the window
        Dimension size_;
        std::string texture_name_;
        TILE_TYPE type_;
};

#endif // ICON_H
