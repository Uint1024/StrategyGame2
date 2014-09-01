#include "Node.h"

Node::Node(Point position, Uint16 g_score, Uint16 f_score) : Tile(position, false, NODE, "node.png"), g_score_(g_score), f_score_(f_score)
{
    //ctor
}

Node::~Node()
{
    //dtor
}


void Node::calculate_f_score(const Point& destination)
{
    f_score_=g_score_+calculate_distance_to_goal(destination)*10;
}

void Node::calculate_g_score(const DIRECTION dire)
{
     g_score_+=dire%2==0?10:14; //add 14 when moving diagonally
}


const float Node::calculate_distance_to_goal(const Point& destination) const
{

    Point difference;

    difference.x = std::abs(position_.x - destination.x);
    difference.y = std::abs(position_.y - destination.y);

    //(c² = a² + b²)
    //int distance=static_cast<int>(sqrt(difference.x * difference.x + difference.y * difference.y));

    //Chebyshev distance
    float distance = (difference.x + difference.y) + (1.41f - 2) * std::min(difference.x, difference.y);
    //std::cout << distance << std::endl;
    return(distance);
}

bool Node::operator<(const Node& b) const
{
  return (f_score_ > b.f_score_);
}


Uint16 Node::getGScore() const { return g_score_; }
Uint16 Node::getFScore() const { return f_score_; }
Uint8 Node::getNodeStatus() const { return node_status_; }


void Node::setStatus(const Uint8 status) {node_status_ = status;}
Uint8 Node::getStatus() const { return node_status_;}
