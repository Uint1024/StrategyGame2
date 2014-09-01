#include "Graphics.h"

Graphics::Graphics() :  window_size_{640,480},
                        sdl_window_{SDL_CreateWindow(
                                    "Strategy game c++ SDL 2.0",
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    640, 480, 0)},
                        sdl_renderer_{SDL_CreateRenderer(
                                    sdl_window_, 0,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE)}

{
    std::cout << "graphics initialized" << std::endl;
}

Graphics::~Graphics()
{
    SDL_DestroyWindow(sdl_window_);
    SDL_DestroyRenderer(sdl_renderer_);
}

void Graphics::drawTiles(Config& config, std::vector<Tile>& world_map, std::vector<Node*>& pathfinding_nodes)
{
    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);

    //TODO : set a real size for the map...
    for(int x = 0 ; x < config.getMapSize().x ; x++)
    {
        SDL_RenderDrawLine(sdl_renderer_, x * config.getTileSize().x - camera_.getPosition().x, 0, x * config.getTileSize().x - camera_.getPosition().x, window_size_.y);
    }

    for(int y = 0 ; y < config.getMapSize().y ; y ++)
    {
        SDL_RenderDrawLine(sdl_renderer_, 0, y * config.getTileSize().y - camera_.getPosition().y, window_size_.x, y * config.getTileSize().y - camera_.getPosition().y);
    }

    for(auto tile : world_map)
    {
        Point tile_position = tile.getPosition() * config.getTileSize() - camera_.getPosition();
        if(tile.getType() == WALL)
        {
            SDL_Rect* tile_rect = new SDL_Rect{ tile_position.x,
                                                tile_position.y,
                                                config.getTileSize().x,
                                                config.getTileSize().y};
            SDL_RenderFillRect(sdl_renderer_, tile_rect);

            delete tile_rect;
        }
    }

    for(auto node : pathfinding_nodes)
    {
        SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0xFF, 0x00, 0xFF);
        SDL_Rect* node_rect = new SDL_Rect{ node->getPosition().x * config.getTileSize().x,
                                                node->getPosition().y * config.getTileSize().y,
                                                config.getTileSize().x, config.getTileSize().y};
        SDL_RenderFillRect(sdl_renderer_, node_rect);

    }
}

void Graphics::drawWindows(Window& windo) const
{
    Color color = windo.getColor();
    SDL_SetRenderDrawColor(sdl_renderer_, color.r, color.g, color.b, 0xFF);

    SDL_Rect* rect = new SDL_Rect{windo.getPosition().x, windo.getPosition().y, windo.getSize().x, windo.getSize().y};
    SDL_RenderFillRect(sdl_renderer_, rect);

    delete rect;
}

void Graphics::drawNodes(int (&open_nodes_map)[200][200], int (&closed_nodes_map)[200][200])
{
    /*SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0xFF, 0xAA, 0xFF);
    for(int i = 0 ; i < 200 ; i++)
    {
        for(int j = 0 ; j < 200 ; j++)
        {
            if(open_nodes_map[i][j] == 1)
            {
                 SDL_Rect* rect = new SDL_Rect{i * 20 - camera_.getPosition().x, j * 20 - camera_.getPosition().y, 5, 5};
                 SDL_RenderFillRect(sdl_renderer_, rect);

                delete rect;
            }

        }
    }*/
}
void Graphics::drawNpcs(Config& config, std::vector<Peasant>& npcs_list) const
{


    for(auto npc : npcs_list)
    {
        SDL_SetRenderDrawColor(sdl_renderer_, 0xAA, 0x0F, 0xAA, 0xFF);
        Point tile_position = npc.getPosition() * config.getTileSize() - camera_.getPosition();
        SDL_Rect* tile_rect = new SDL_Rect{ tile_position.x,
                                            tile_position.y,
                                            config.getTileSize().x,
                                            config.getTileSize().y};
        SDL_RenderFillRect(sdl_renderer_, tile_rect);

        delete tile_rect;

        if(npc.getGoal() != npc.getPosition())
        {
            SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0x0F, 0x00, 0xFF);
            tile_position = npc.getGoal() * config.getTileSize() - camera_.getPosition();
            tile_rect = new SDL_Rect{ tile_position.x,
                                                tile_position.y,
                                                config.getTileSize().x,
                                                config.getTileSize().y};
            SDL_RenderFillRect(sdl_renderer_, tile_rect);

            delete tile_rect;
        }

    }


}

void Graphics::clear()
{
    SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xFF, 0xAA, 0xFF);
    SDL_RenderClear(sdl_renderer_);


}

void Graphics::flip()
{
    SDL_RenderPresent(sdl_renderer_);
}

void Graphics::moveCamera(const IVector& movement)
{
    camera_.moveCamera(movement);
}

Camera Graphics::getCamera() const { return camera_; }
