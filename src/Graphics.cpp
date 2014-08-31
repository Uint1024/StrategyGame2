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
    for(int x = 0 ; x < 2000 ; x += config.getTileSize().x)
    {
        SDL_RenderDrawLine(sdl_renderer_, x - camera_.getPosition().x, 0, x - camera_.getPosition().x, window_size_.y);
    }

    for(int y = 0 ; y < 2000 ; y += config.getTileSize().y)
    {
        SDL_RenderDrawLine(sdl_renderer_, 0, y - camera_.getPosition().y, window_size_.x, y  - camera_.getPosition().y);
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
