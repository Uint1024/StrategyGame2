#include "Stockpile.h"
#include "Peasant.h"

Stockpile::Stockpile(Point position) : GameObject(position, Dimension{20,20}, false, STOCKPILE, "stockpile.png")
{
    //ctor
}

Stockpile::~Stockpile()
{
    //dtor
}

void Stockpile::storeRessources(Peasant* peasant)
{
    wood_ += peasant->getWood();
    peasant->emptyInventory();
    std::cout << "The stockpile contains " << wood_ << " wood" << std::endl;
}

void Stockpile::takeWood(Peasant* peasant)
{
    int wood_to_take = peasant->getNeededWood() - peasant->getWood();
    if(wood_to_take + peasant->getWood() > peasant->getMaxWood())
        wood_to_take = peasant->getMaxWood() - peasant->getWood();
    wood_ -= wood_to_take;
    peasant->addWood(wood_to_take);
}


int Stockpile::getWood() const { return wood_; }
