#include "Peasant.h"
#include "Tree.h"

Peasant::Peasant(Point position) : Entity(position, Dimension{20,20}, "peasant.png"), goal_(position)
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

void Peasant::move(std::vector<Peasant>& npc_list)
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

void Peasant::update()
{
    /*//std::cout << "last order = " << time_of_last_order_ << std::endl;
    int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    if(list_of_pathfinding_movements_.size() > 0 &&
       position_in_pathfinding_ < list_of_pathfinding_movements_.size())
    {

        position_.x += xMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];
        position_.y += yMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];

        position_in_pathfinding_ += 1;
    }*/

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
            std::cout << next_position_.y << std::endl;

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
    DIRECTION position_to_goal_;
    random_goals_tried += 1;


    if(position_.x > goal_.x)
    {
        if(position_.y > goal_.y)
            position_to_goal_ = DOWN_RIGHT;
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

    std::cout << random_goals_tried << std::endl;

    goal_ = goal_ + Point{xMovement[position_to_goal_],yMovement[position_to_goal_]};
}

void Peasant::cutWood(std::shared_ptr<Tree> tree)
{
    std::cout << "I'm cutting wood" << std::endl;

    tree->sayHi();
}

int Peasant::getRandomGoalsTried()
{
    return random_goals_tried;
}
void Peasant::setPathfinding(std::vector<DIRECTION> list_of_pathfinding_movements)
{
    position_in_pathfinding_ = 0;
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



