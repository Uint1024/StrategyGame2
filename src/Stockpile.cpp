#include "Stockpile.h"
#include "Peasant.h"

Stockpile::Stockpile(Point position) : GameObject(position, Dimension{20,20}, true, STOCKPILE, "stockpile.png")
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
