#ifndef PLAINS_H
#define PLAINS_H

#include "GameObject.h"
#include <SDL.h>
class Plains : public GameObject
{
    public:
        Plains(Point position);
        virtual ~Plains();
    protected:
    private:
};

#endif // PLAIN_H
