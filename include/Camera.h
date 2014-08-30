#ifndef CAMERA_H
#define CAMERA_H
#include <SDL.h>
#include "Coordinate.h"
#include <iostream>

class Camera
{
    public:
        Camera();
        virtual ~Camera();
        Point getPosition() const;
        void moveCamera(const IVector& movement) ;
    protected:
    private:
        Point position_;
        float zoom_;
};

#endif // CAMERA_H
