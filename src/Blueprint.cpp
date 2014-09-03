#include "Blueprint.h"

Blueprint::Blueprint(Point position, TILE_TYPE type, std::string texture_name) : GameObject(position, Dimension{20,20}, false, type, texture_name), built_(false)
{
   if(type == WOOD_WALL_BLUEPRINT)
        wood_needed_ = 30;
}

Blueprint::~Blueprint()
{
    //dtor
}


bool Blueprint::changeWoodNeeded(int wood)
{
    wood_needed_ -= wood;
    std::cout << "Still " << wood_needed_ << " wood needed" << std::endl;
    if(wood_needed_ > 0) return true;
    else
    {
        built_ = true;
        return false;
    }
}

int Blueprint::getWoodNeeded() const { return wood_needed_; }
bool Blueprint::isBuilt() const { return built_; }
