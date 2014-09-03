#include "Peasant.h"
#include "Tree.h"
#include "Blueprint.h"

Peasant::Peasant(Point position) :  GameObject(position, Dimension{20,20}, true, PEASANT, "peasant.png"),
                                    goal_(position),
                                    object_goal_(nullptr),
                                    wood_(0),
                                    current_activity_(NOTHING),
                                    previous_activity_(NOTHING)
{
position_in_pathfinding_ = 0;
time_of_last_order_ = 0;
next_position_ = position_;
random_goals_tried = 0;
needed_wood_ = 0;
max_wood_ = 10;
}

Peasant::~Peasant()
{
    //dtor
}

void Peasant::update(std::vector<Peasant>& npc_list, std::vector<std::shared_ptr<GameObject>> (&world_map)[2])
{
    if(goal_ != position_)
    {
        int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
        int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};
        Point temp_pos = position_;
        bool bumping_into_npc = false;
        if(list_of_pathfinding_movements_.size() > 0 &&
           position_in_pathfinding_ < list_of_pathfinding_movements_.size())
        {

            temp_pos.x += xMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];
            temp_pos.y += yMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];

            position_in_pathfinding_ += 1;
        }

        for(auto npc : npc_list)
        {
            if(temp_pos == npc.getPosition())
            {
                bumping_into_npc = true;

            }
        }

        if(!bumping_into_npc)
        {
            position_ = temp_pos;
        }
    }

    bool wood_available_ = false;
    //scanning every object to find stockpile, check if there are any building material available
    for(auto it = world_map[1].begin() ; it != world_map[1].end() ; it++)
    {
        if((*it) != nullptr)
        {
            if((*it)->getType() == STOCKPILE &&
               std::dynamic_pointer_cast<Stockpile>(*it)->getWood() > 0)
                wood_available_ = true;
        }
    }


    for(auto it = world_map[1].begin() ; it != world_map[1].end() ; it++)
    {
        if((*it) != nullptr)
        {
            if((*it)->getType() == WOOD_WALL_BLUEPRINT &&
               wood_available_ == true)
            {
                current_activity_ = BUILDING;
                findClosest(world_map, WOOD_WALL_BLUEPRINT);
            }
            else if ((*it)->getType() == WOOD_WALL_BLUEPRINT &&
               wood_available_ == false)
            {
                std::cout << "no wood available in stockpiles :(" << std::endl;
                //previous_activity_ = current_activity_;
                //current_activity_ = CUTTING_TREES;

            }
        }
    }

    if(wood_ >= max_wood_ && current_activity_ != BUILDING)
    {
        findClosest(world_map, STOCKPILE);
        current_activity_ = STOCKPILING;
        previous_activity_ = CUTTING_TREES;
    }



    if(current_activity_ == BUILDING)
    {
        needed_wood_ = std::dynamic_pointer_cast<Blueprint>(building_goal_)->getWoodNeeded();

        if(needed_wood_ > wood_ && wood_ < max_wood_)
        {
            current_activity_ = GETTING_STUFF_FROM_STOCKPILE;
            previous_activity_ = BUILDING;
            findClosest(world_map, STOCKPILE);
        }
        else
        {
            goal_ = building_goal_->getPosition();

            object_goal_ = building_goal_;
        }

    }

    if(object_goal_ != nullptr)
    {
        Point diff = object_goal_->getPosition() - position_;
        diff.x = std::abs(diff.x);
        diff.y = std::abs(diff.y);

        if(diff.x < 2 && diff.y < 2)
        {
            if(current_activity_ == CUTTING_TREES
               && object_goal_->getType() == TREE)
            {
                if(std::dynamic_pointer_cast<Tree>(object_goal_)->mineRessources(this) <= 0)
                {
                    object_goal_ = nullptr;
                    findClosest(world_map, TREE);
                }
            }
            else if(current_activity_ == STOCKPILING
                    && object_goal_->getType() == STOCKPILE)
            {
                std::dynamic_pointer_cast<Stockpile>(object_goal_)->storeRessources(this);

                if(previous_activity_ == CUTTING_TREES)
                {
                    current_activity_ = CUTTING_TREES;
                    previous_activity_ = STOCKPILING;
                    findClosest(world_map, TREE);
                }
            }
            else if(current_activity_ == GETTING_STUFF_FROM_STOCKPILE
                    && object_goal_->getType() == STOCKPILE)
            {
                std::dynamic_pointer_cast<Stockpile>(object_goal_)->takeWood(this);
                current_activity_ = BUILDING;
                previous_activity_ = GETTING_STUFF_FROM_STOCKPILE;
            }
            else if(current_activity_ == BUILDING
                    && object_goal_->getType() == WOOD_WALL_BLUEPRINT)
            {
                auto ptr_to_building = std::dynamic_pointer_cast<Blueprint>(object_goal_);
                int wood_to_remove = ptr_to_building->getWoodNeeded();
                if(wood_to_remove > wood_) wood_to_remove = wood_;

                bool wood_still_needed = std::dynamic_pointer_cast<Blueprint>(object_goal_)->changeWoodNeeded(wood_to_remove);
                wood_ -= wood_to_remove;


                previous_activity_ = BUILDING;
                if(wood_still_needed)
                {
                    current_activity_ = GETTING_STUFF_FROM_STOCKPILE;

                }
                else
                {
                    current_activity_ = NOTHING;

                }

            }

        }
    }
}

void Peasant::findClosest(std::vector<std::shared_ptr<GameObject>> (&world_map)[2], TILE_TYPE object_to_find)
{
    std::shared_ptr<GameObject> closestObject = nullptr;
    float distance_closest = 0;
    float distance = 0;
    for(auto it = world_map[1].begin() ; it != world_map[1].end() ; ++it)
    {
        if((*it) != nullptr && (*it)->getType() == object_to_find)
        {
            Point difference;

            difference.x = std::abs(position_.x - (*it)->getPosition().x);
            difference.y = std::abs(position_.y - (*it)->getPosition().y);

            if((object_to_find == STOCKPILE &&
                current_activity_ == STOCKPILING &&
                std::dynamic_pointer_cast<Stockpile>(*it)->getWood() <= 40) ||

               (object_to_find == STOCKPILE &&
                current_activity_ == GETTING_STUFF_FROM_STOCKPILE &&
                std::dynamic_pointer_cast<Stockpile>(*it)->getWood() > 0) ||

                object_to_find != STOCKPILE)
            {
                if(closestObject == nullptr)
                {
                    closestObject = (*it);
                    distance_closest = (difference.x + difference.y) - 0.6f * std::min(difference.x, difference.y);
                }
                else
                {
                    distance = (difference.x + difference.y) - 0.6f * std::min(difference.x, difference.y);
                    if(distance < distance_closest)
                    {
                        closestObject = (*it);
                        distance_closest = distance;
                    }
                }
            }
        }
    }

    if(closestObject != nullptr)
    {
        if(current_activity_ == BUILDING)
        {
            building_goal_ = closestObject;
            goal_ = closestObject->getPosition();
        }
        else
        {
            object_goal_ = closestObject;
            goal_ = closestObject->getPosition();
        }

    }
    else
    {
        std::cout << "Can't find object corresponding to current activity!" << std::endl;
        current_activity_ = NOTHING;
        object_goal_ = nullptr;
        building_goal_ = nullptr;
    }
}


void Peasant::emptyInventory()
{
    wood_ = 0;
}


void Peasant::addWood(const int quantity)
{
    wood_ += quantity;
    std::cout << "I have " << wood_ << " wood" << std::endl;
}


std::shared_ptr<GameObject> Peasant::getObjectGoal() const
{
    return  object_goal_;
}

void Peasant::calculateMovement(std::vector<Peasant>& npc_list)
{
    int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    if(list_of_pathfinding_movements_.size() > 0 &&
       position_in_pathfinding_ < list_of_pathfinding_movements_.size())
    {

        next_position_.x += xMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];
        next_position_.y += yMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];

        position_in_pathfinding_ += 1;
    }


    for(auto npc : npc_list)
    {
        if(next_position_ == npc.getPosition())
        {

            next_position_ = position_;
        }
    }


}

Point Peasant::getNextPosition() const {return next_position_; }


void Peasant::setGoal(const Point& goal) { goal_ = goal;}
void Peasant::noGoal() {
    goal_ = position_;

    random_goals_tried = 0;
};

void Peasant::newRandomGoal()
{

    int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    if(random_goals_tried >= 1)
        goal_ = goal_ - Point{xMovement[position_to_goal_],yMovement[position_to_goal_]};



    random_goals_tried += 1;

    /*if(random_goals_tried <=1 )
    {
        if(position_.x > goal_.x)
        {
            if(position_.y > goal_.y)
            {
                switch(random_goals_tried)
                {
                    case 1:
                         position_to_goal_ = DOWN_RIGHT;
                         break;
                }
            }

            if(position_.y < goal_.y)
                position_to_goal_ = UP_RIGHT;
            if(position_.y == goal_.y)
                position_to_goal_ = RIGHT;
        }
        else if(position_.x == goal_.x)
        {
            if(position_.y > goal_.y)
                position_to_goal_ = DOWN;
            if(position_.y < goal_.y)
                position_to_goal_ = UP;
        }
        else if(position_.x < goal_.x)
        {
            if(position_.y > goal_.y)
                position_to_goal_ = DOWN_LEFT;
            if(position_.y < goal_.y)
                position_to_goal_ = UP_LEFT;
            if(position_.y == goal_.y)
                position_to_goal_ = LEFT;

        }
    }
    else
    {*/
        position_to_goal_ = static_cast<DIRECTION>(random_goals_tried);
    //}

    goal_ = goal_ + Point{xMovement[position_to_goal_],yMovement[position_to_goal_]};
}


int Peasant::getRandomGoalsTried()
{
    return random_goals_tried;
}

void Peasant::setPathfinding(std::vector<DIRECTION> list_of_pathfinding_movements)
{
    position_in_pathfinding_ = 0;
    random_goals_tried = 0;
     list_of_pathfinding_movements_ = list_of_pathfinding_movements;
};

Uint32 Peasant::getTimeOfLastOrder() const
{

    return time_of_last_order_;
}
void Peasant::setTimeOfLastOrder(const Uint32 time) {
    time_of_last_order_ = time;
}

Point Peasant::getGoal() const { return goal_; }
void Peasant::setObjectGoal(std::shared_ptr<GameObject> object)
{
    object_goal_ = object;
    if(object_goal_ != nullptr)
    {
        if(object_goal_->getType() == TREE)
        {
            current_activity_ = CUTTING_TREES;
        }
    }
}

int Peasant::getWood() const { return wood_; }
int Peasant::getMaxWood() const { return max_wood_; }
int Peasant::getNeededWood() const { return needed_wood_; }
