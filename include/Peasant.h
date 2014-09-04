#ifndef PEASANT_H
#define PEASANT_H
#include <vector>
#include <memory>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include "Entity.h"
#include "Coordinate.h"
#include "GameObject.h"
#include "time.h"
#include "Stockpile.h"

class Tree;
class Peasant : public GameObject
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
        void update(std::vector<Peasant>& npc_list, std::vector<std::shared_ptr<GameObject>> (&world_map)[2]);
        void findClosestStockpile(std::vector<std::shared_ptr<GameObject>> (&world_map)[2]);
        void newRandomGoal();
        int getRandomGoalsTried();
        void cutWood(std::shared_ptr<Tree> tree);
        void setObjectGoal(std::shared_ptr<GameObject> object);
        void emptyInventory();
        void addWood(const int quantity);
        //std::shared_ptr<Tree> setObjectGoal() const; //object to mine, cut, or work on
        std::shared_ptr<GameObject> getObjectGoal() const;
        void findClosest(std::vector<std::shared_ptr<GameObject>> (&world_map)[2], TILE_TYPE object_to_find);
        int getWood() const;
        int getMaxWood() const;
        int getNeededWood() const;


    protected:
    private:
        Point next_position_;
        std::vector<DIRECTION> list_of_pathfinding_movements_;
        Point goal_;
        Uint16 position_in_pathfinding_;
        Uint32 time_of_last_order_;
        int random_goals_tried;
        //std::shared_ptr<Tree> ressource_goal_; //object to mine, cut, or work on
        //std::shared_ptr<Stockpile> stockpile_goal_;
        std::shared_ptr<GameObject> object_goal_;
        std::shared_ptr<GameObject> building_goal_;
        int wood_;
        DIRECTION position_to_goal_;
        ACTIVITY current_activity_;
        ACTIVITY previous_activity_;
        int needed_wood_;
        int max_wood_;
        OCCUPATION occupation_;
};

#endif // PEASANT_H
