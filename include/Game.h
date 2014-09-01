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
        bool findPath(Peasant& peasant);
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
        Uint32 chrono_for_pathfinding_;
        Uint32 chrono_movement_;
        //int closed_nodes_map[200][200];
        //int vector<int> closed_nodes_map_;
        //int open_nodes_map[200][200];
        //int dir_map[200][200];
        //bool solid_map_[200][200];
        std::vector<bool> solid_map_;
        float path_finding_elapsed_time; //don't search for more than 1/4th of a second
        float path_finding_time_last_frame;
        int searched_tiles;
        float last_time_npc_creation_;

};

#endif // GAME_H
