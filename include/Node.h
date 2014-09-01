#ifndef NODE_H
#define NODE_H

#include <SDL.h>
#include <math.h>
#include <cstdlib>
#include "Tile.h"
/*Node for the pathfinding algorithm*/

class Node : public Tile
{
    public:
        Node(Point position, Uint16 g_score, Uint16 f_score);
        virtual ~Node();
        bool operator<(const Node& b) const;
        Uint16 getGScore() const;
        Uint16 getFScore() const;
        Uint8 getNodeStatus() const;
        void calculate_f_score(const Point& destination);
        void calculate_g_score(const DIRECTION dire);
        const float calculate_distance_to_goal(const Point& destination) const;
        void setStatus(const Uint8 status);
        Uint8 getStatus() const;
    protected:
    private:
        Uint16 g_score_, f_score_; //g_score_ is the distance from the start, f_score_ is the distance to the end
        Uint8 node_status_; //0 : not visited ; 1 : open ; 2 : closed
};

#endif // NODE_H
