#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL.h>
#include <iostream>
#include <vector>
#include <SDL_image.h>
#include <stdio.h>

#include "Coordinate.h"
#include "Config.h"
//#include "tile.h"
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
        void drawWindow(const Window* windo, const std::vector<Icon>& icon_list);
        void drawIcon(const Icon& icon) const;
        void drawNpcs(Config& config, std::vector<Peasant>& npcs_list);
        void drawNodes(int (&open_nodes_map)[200][200], int (&closed_nodes_map)[200][200]);
        SDL_Texture* loadImage(std::string file_name);
    protected:
    private:
        SDL_Window* sdl_window_;
        SDL_Renderer* sdl_renderer_;
        Dimension window_size_;
        Camera camera_;
        std::map<std::string, SDL_Texture*> sprite_sheets_;
};

#endif // GRAPHICS_H
