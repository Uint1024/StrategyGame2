#ifndef COORDINATE_H_INCLUDED
#define COORDINATE_H_INCLUDED

enum TILE_TYPE{ EMPTY, WALL, NODE, PLAINS};
enum DIRECTION {RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
//enum DIRECTION_X {RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
//enum DIRECTIO_Y {RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
struct Point
{
    int x;
    int y;

    void operator+=(const Point& b)
    {
        x += b.x;
        y += b.y;
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

//inline bool operator!=(const Point& a, const Point& b){return true;}


typedef Point Dimension;
typedef Point IVector;



struct Coordinate
{
    float x;
    float y;
};


#endif // COORDINATE_H_INCLUDED
