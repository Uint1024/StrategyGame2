#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "Graphics.h"
#include "Inputs.h"
#include "Tile.h"
#include "Wall.h"
#include <vector>
#include <algorithm>
#include <queue>
#include "Node.h"
#include "Plains.h"

class Game
{
    public:
        Game();
        virtual ~Game();
        void gameLoop();
        void update(SDL_Event& event);
        void draw(Graphics& graphics, Config& config);
        void receiveInputs(SDL_Event& event);
        std::string findPath(const Point start, const Point finish);
    protected:
    private:
        Graphics graphics_;
        Inputs inputs_;
        Config config_;
        bool playing_;
        std::vector<Tile> world_map_;
        Point start;
        Point finish;
        std::vector<Node*> pathfinding_nodes_;
};

#endif // GAME_H
