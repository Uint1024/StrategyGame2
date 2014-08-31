#ifndef PEASANT_H
#define PEASANT_H
#include <vector>
#include "Entity.h"
#include "Coordinate.h"
#include "Tile.h"

class Peasant : public Entity
{
    public:
        Peasant(Point position);
        virtual ~Peasant();
        void setGoal(const Point& goal);
        void update();
        void setPathfinding(std::vector<DIRECTION> list_of_pathfinding_movements);
        Point getGoal() const;
        float getTimeSinceLastOrder() const;
        void setTimeSinceLastOrder();
    protected:
    private:
        std::vector<DIRECTION> list_of_pathfinding_movements_;
        Point goal_;
        Uint16 position_in_pathfinding_;
        float time_since_last_order_;
};

#endif // PEASANT_H
