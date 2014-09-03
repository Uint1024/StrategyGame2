#ifndef BLUEPRINT_H
#define BLUEPRINT_H
#include <iostream>
#include "GameObject.h"

class Blueprint : public GameObject
{
    public:
        Blueprint(Point position, TILE_TYPE type, std::string texture_name);
        virtual ~Blueprint();
        int getWoodNeeded() const;
        bool changeWoodNeeded(int wood);
        bool isBuilt() const;
    protected:
        TILE_TYPE type_finished_construction_;
        int wood_needed_;
        bool built_;
    private:
};

#endif // BLUEPRINT_H
