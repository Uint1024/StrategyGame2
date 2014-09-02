#ifndef PEASANT_H
#define PEASANT_H
#include <vector>
#include <memory>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include "Entity.h"
#include "Coordinate.h"
#include "Tile.h"
#include "time.h"

class Tree;
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
        void update(std::vector<Peasant>& npc_list, std::vector<std::shared_ptr<Tile>> (&world_map)[2]);
        void newRandomGoal();
        int getRandomGoalsTried();
        void cutWood(std::shared_ptr<Tree> tree);
        void setRessourceGoal(std::shared_ptr<Tree> tree);
        void addWood(const int quantity);
        std::shared_ptr<Tree> getRessourceGoal() const; //object to mine, cut, or work on
        void findClosestTree(std::vector<std::shared_ptr<Tile>> (&world_map)[2]);
    protected:
    private:
        Point next_position_;
        std::vector<DIRECTION> list_of_pathfinding_movements_;
        Point goal_;
        Uint16 position_in_pathfinding_;
        Uint32 time_of_last_order_;
        int random_goals_tried;
        std::shared_ptr<Tree> ressource_goal_; //object to mine, cut, or work on
        int wood_;
        DIRECTION position_to_goal_;
};

#endif // PEASANT_H
