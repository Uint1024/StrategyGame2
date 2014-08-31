#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL.h>
#include <iostream>
#include <vector>

#include "Coordinate.h"
#include "Config.h"
#include "tile.h"
#include "Node.h"
#include "Camera.h"
#include "Window.h"
#include "Peasant.h"

class Graphics
{
    public:
        Graphics();
        virtual ~Graphics();

        void flip();
        void clear();
        void drawTiles(Config& config, std::vector<Tile>& world_map, std::vector<Node*>& pathfinding_nodes);
        void moveCamera(const IVector& movement);
        Camera getCamera() const;
        void drawWindows(Window& windo) const;
        void drawNpcs(Config& config, std::vector<Peasant>& npcs_list) const;
    protected:
    private:
        SDL_Window* sdl_window_;
        SDL_Renderer* sdl_renderer_;
        Dimension window_size_;
        Camera camera_;
};

#endif // GRAPHICS_H
