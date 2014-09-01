#ifndef PEASANT_H
#define PEASANT_H
#include <vector>
#include <iostream>
#include "Entity.h"
#include "Coordinate.h"
#include "Tile.h"
#include "time.h"
class Peasant : public Entity
{
    public:
        Peasant(Point position);
        virtual ~Peasant();
        void setGoal(const Point& goal);
        void update();
        void setPathfinding(std::vector<DIRECTION> list_of_pathfinding_movements);
        Point getGoal() const;
        void setTimeOfLastOrder(Uint32 time);
        void noGoal();
        Uint32 getTimeOfLastOrder() const;
        void calculateMovement(std::vector<Peasant>& npc_list);
        Point getNextPosition() const;
        void move(std::vector<Peasant>& npc_list);
        void newRandomGoal();
        int getRandomGoalsTried();
    protected:
    private:
        Point next_position_;
        std::vector<DIRECTION> list_of_pathfinding_movements_;
        Point goal_;
        Uint16 position_in_pathfinding_;
        Uint32 time_of_last_order_;
        int random_goals_tried;
};

#endif // PEASANT_H
