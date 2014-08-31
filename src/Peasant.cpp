#include "Peasant.h"

Peasant::Peasant(Point position) : Entity(position, Dimension{20,20})
{
position_in_pathfinding_ = 0;
}

Peasant::~Peasant()
{
    //dtor
}

void Peasant::update()
{
    int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    if(list_of_pathfinding_movements_.size() > 0 &&
       position_in_pathfinding_ < list_of_pathfinding_movements_.size())
    {

        position_.x += xMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];
        position_.y += yMovement[list_of_pathfinding_movements_[position_in_pathfinding_]];

        position_in_pathfinding_ += 1;
    }

}
void Peasant::setGoal(const Point& goal) { goal_ = goal;}

void Peasant::setPathfinding(std::vector<DIRECTION> list_of_pathfinding_movements)
{
        position_in_pathfinding_ = 0;
     list_of_pathfinding_movements_ = list_of_pathfinding_movements;
};

float Peasant::getTimeSinceLastOrder() const
{
    return (SDL_GetTicks() - time_since_last_order_);
}
Point Peasant::getGoal() const { return goal_; }
void Peasant::setTimeSinceLastOrder() { time_since_last_order_ = SDL_GetTicks(); }
