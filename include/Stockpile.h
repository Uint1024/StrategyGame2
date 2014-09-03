#ifndef STOCKPILE_H
#define STOCKPILE_H
#include "GameObject.h"
class Peasant;
class Stockpile : public GameObject
{
    public:
        Stockpile(Point position);
        virtual ~Stockpile();
        void storeRessources(Peasant* peasant);
    protected:
        int wood_;
    private:
};

#endif // STOCKPILE_H
