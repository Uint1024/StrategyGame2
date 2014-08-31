#ifndef COORDINATE_H_INCLUDED
#define COORDINATE_H_INCLUDED
#include <SDL.h>

enum TILE_TYPE{ EMPTY, WALL, NODE, PLAINS};
enum DIRECTION {RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
//enum DIRECTION_X {RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
//enum DIRECTIO_Y {RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };

    //int xMovement[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    //int yMovement[8] = {0, 1, 1, 1, 0, -1, -1, -1};

struct Color
{
    int r;
    int g;
    int b;
};

struct Point
{
    int x;
    int y;

    void operator+=(const Point& b)
    {
        x += b.x;
        y += b.y;
    }

    bool intersect(const SDL_Rect& rect) const
    {
        if(x >= rect.x && x < rect.x + rect.w &&
           y >= rect.y && y < rect.y + rect.h)
            return true;
        else
            return false;
    }
    Point operator-(const Point& b) const {return Point{x - b.x, y - b.y};}
};

//inline Point operator-(const Point& a, const Point& b){return Point{a.x - b.x, a.y - b.y};}

inline Point operator+(const Point& a, const Point& b){return Point{a.x + b.x, a.y + b.y};}
inline Point operator+=(const Point& a, const Point& b){return Point{a.x + b.x, a.y + b.y};}

inline Point operator/(const Point& a, const Point& b){return Point{a.x / b.x, a.y / b.y};}


inline Point operator%(const Point& a, const Point& b){return Point{a.x % b.x, a.y % b.y};}


inline Point operator*(const Point& a, const Point& b){return Point{a.x * b.x, a.y * b.y};}


inline bool operator==(const Point& a, const Point& b){ return (a.x == b.x && a.y == b.y) ? true :  false;}

inline bool operator!=(const Point& a, const Point& b){ return (a.x != b.x || a.y != b.y) ? true :  false;}



typedef Point Dimension;
typedef Point IVector;



struct Coordinate
{
    float x;
    float y;
};


#endif // COORDINATE_H_INCLUDED
