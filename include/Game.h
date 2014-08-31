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
#include "Window.h"
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
        Window world_editor_;
        std::vector<Peasant> npcs_list_;
        float get_ticks_previous_frame_;
        float elapsed_time_;
        float fps_;
        Uint32 chrono_for_pathfinding_test_;
        int closed_nodes_map[200][200];
        int open_nodes_map[200][200];
        int dir_map[200][200];
        bool solid_map_[200][200];
};

#endif // GAME_H
