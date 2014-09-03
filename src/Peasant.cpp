#include "Peasant.h"
#include "Tree.h"

Peasant::Peasant(Point position) :  GameObject(position, Dimension{20,20}, true, PEASANT, "peasant.png"),
                                    goal_(position),
                                    ressource_goal_(nullptr),
                                    stockpile_goal_(nullptr),
                                    wood_(0),
                                    current_activity_(NOTHING),
                                    previous_activity_(NOTHING)
{
position_in_pathfinding_ = 0;
time_of_last_order_ = 0;
next_position_ = position_;
random_goals_tried = 0;
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

    if(wood_ >= 10 && stockpile_goal_ == nullptr)
    {
        ressource_goal_ = nullptr;
        goal_ = position_;
        findClosestStockpile(world_map);
        current_activity_ = STOCKPILING;
        previous_activity_ = CUTTING_TREES;
    }

    if(ressource_goal_ != nullptr)
    {
        Point diff = ressource_goal_->getPosition() - position_;
        diff.x = std::abs(diff.x);
        diff.y = std::abs(diff.y);
        if(diff.x < 2 && diff.y < 2)
        {
            if(ressource_goal_->mineRessources(this) <= 0)
            {
                ressource_goal_ = nullptr;
                findClosestTree(world_map);
            }

        }
    }



    if(current_activity_ == STOCKPILING)
    {
        Point diff{ std::abs(stockpile_goal_->getPosition().x - position_.x),
                    std::abs(stockpile_goal_->getPosition().y - position_.y)};
        if(diff.x < 2 && diff.y < 2)
        {
            stockpile_goal_->storeRessources(this);

            if(previous_activity_ == CUTTING_TREES)
            {
                current_activity_ = CUTTING_TREES;
                previous_activity_ = STOCKPILING;
                goal_ = position_;
                stockpile_goal_ = nullptr;
                findClosestTree(world_map);
            }
        }
    }
}

void Peasant::findClosestTree(std::vector<std::shared_ptr<GameObject>> (&world_map)[2])
{
    std::shared_ptr<Tree> closestTree = nullptr;
    float distance_closest = 0;
    float distance = 0;
    for(auto it = world_map[1].begin() ; it != world_map[1].end() ; ++it)
    {
        if((*it) != nullptr && (*it)->getType() == TREE)
        {
            Point difference;

            difference.x = std::abs(position_.x - (*it)->getPosition().x);
            difference.y = std::abs(position_.y - (*it)->getPosition().y);

            if(closestTree == nullptr)
            {
                closestTree = std::dynamic_pointer_cast<Tree>(*it);
                distance_closest = (difference.x + difference.y) - 0.6f * std::min(difference.x, difference.y);
            }
            else
            {
                distance = (difference.x + difference.y) - 0.6f * std::min(difference.x, difference.y);
                if(distance < distance_closest)
                {
                    closestTree = std::dynamic_pointer_cast<Tree>(*it);
                    distance_closest = distance;
                }
            }
        }
    }

    if(closestTree != nullptr)
    {
        current_activity_ = CUTTING_TREES;
        ressource_goal_ = closestTree;
        goal_ = closestTree->getPosition();

    }
    else
    {
        current_activity_ = NOTHING;
    }
}

void Peasant::emptyInventory()
{
    wood_ = 0;
}
void Peasant::findClosestStockpile(std::vector<std::shared_ptr<GameObject>> (&world_map)[2])
{
    std::shared_ptr<Stockpile> closest_stockpile = nullptr;
    float distance_closest = 0;
    float distance = 0;
    for(auto it = world_map[1].begin() ; it != world_map[1].end() ; ++it)
    {
        if((*it) != nullptr && (*it)->getType() == STOCKPILE)
        {
            Point difference;

            difference.x = std::abs(position_.x - (*it)->getPosition().x);
            difference.y = std::abs(position_.y - (*it)->getPosition().y);

            if(closest_stockpile == nullptr)
            {
                closest_stockpile = std::dynamic_pointer_cast<Stockpile>(*it);
                distance_closest = (difference.x + difference.y) - 0.6f * std::min(difference.x, difference.y);
            }
            else
            {
                distance = (difference.x + difference.y) - 0.6f * std::min(difference.x, difference.y);
                if(distance < distance_closest)
                {
                    closest_stockpile = std::dynamic_pointer_cast<Stockpile>(*it);
                    distance_closest = distance;
                }
            }
        }
    }

    if(closest_stockpile != nullptr)
    {
        stockpile_goal_ = closest_stockpile;
        goal_ = closest_stockpile->getPosition();
        current_activity_ = STOCKPILING;

    }
    else
    {
        std::cout << "no stockpile available" << std::endl;
    }
}


void Peasant::addWood(const int quantity)
{
    wood_ += quantity;
    std::cout << "I have " << wood_ << " wood" << std::endl;
}


std::shared_ptr<Tree> Peasant::getRessourceGoal() const
{
    return  ressource_goal_;
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
void Peasant::setRessourceGoal(std::shared_ptr<Tree> tree) { ressource_goal_ = tree; }
std::shared_ptr<Stockpile> Peasant::getStockpileGoal() const { return stockpile_goal_; }
int Peasant::getWood() const { return wood_; }
