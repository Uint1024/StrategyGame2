#include "Graphics.h"

Graphics::Graphics() :  window_size_{1000,1000},
                        sdl_window_{SDL_CreateWindow(
                                    "Strategy game c++ SDL 2.0",
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    1000, 1000, 0)},
                        sdl_renderer_{SDL_CreateRenderer(
                                    sdl_window_, 0,
                                    SDL_RENDERER_ACCELERATED |
                                    SDL_RENDERER_PRESENTVSYNC |
                                    SDL_RENDERER_TARGETTEXTURE)},
                        font(TTF_OpenFont("arial.ttf", 25))

{
    std::cout << "graphics initialized" << std::endl;
}

Graphics::~Graphics()
{
    SDL_DestroyWindow(sdl_window_);
    SDL_DestroyRenderer(sdl_renderer_);
}

void Graphics::drawInputs(Inputs& inputs)
{

    if(inputs.getHeldMouseButtons()[SDL_BUTTON_LEFT])
    {
        SDL_SetRenderDrawColor(sdl_renderer_, 200, 200, 150, 250);
        Point diff = inputs.getMouseSelectionEnd() - inputs.getMouseSelectionStart();

        if(diff.x > 0 && diff.y > 0)
        {
           SDL_Rect* rect = new SDL_Rect{   inputs.getMouseSelectionStart().x,
                                            inputs.getMouseSelectionStart().y,
                                            diff.x,
                                            diff.y};
            SDL_RenderDrawRect(sdl_renderer_, rect);

            delete rect;


        }
        else if (diff.x < 0 && diff.y > 0)
        {
            SDL_Rect* rect = new SDL_Rect{  inputs.getMouseSelectionStart().x + diff.x,
                                            inputs.getMouseSelectionStart().y,
                                            -diff.x,
                                            diff.y};

            SDL_RenderDrawRect(sdl_renderer_, rect);

            delete rect;
        }
        else if (diff.x < 0 && diff.y < 0)
        {
            SDL_Rect* rect = new SDL_Rect{  inputs.getMouseSelectionStart().x + diff.x,
                                            inputs.getMouseSelectionStart().y + diff.y,
                                            -diff.x,
                                            -diff.y};

            SDL_RenderDrawRect(sdl_renderer_, rect);

            delete rect;
        }
        else if (diff.x > 0 && diff.y < 0)
        {
            SDL_Rect* rect = new SDL_Rect{  inputs.getMouseSelectionStart().x,
                                            inputs.getMouseSelectionStart().y + diff.y,
                                            diff.x,
                                            -diff.y};

            SDL_RenderDrawRect(sdl_renderer_, rect);

            delete rect;
        }


    }

if( font == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );

    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, "HELLdfsdfsdfsdgsfgsdO", SDL_Color{255, 255, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer_, surface);

std::cout << "wtf";
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {10, 400, texW, texH};

    SDL_RenderCopy(sdl_renderer_, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Graphics::drawTiles(Config& config, std::vector<std::shared_ptr<GameObject>> (&world_map)[2],
                          std::vector<Node*>& pathfinding_nodes)
{


    for(auto &tile : world_map[0])
    {
        Point tile_position = tile->getPosition() * config.getTileSize() - camera_.getPosition();

        SDL_Rect* src_rect = new SDL_Rect{  0,
                                            0,
                                            config.getTileSize().x,
                                            config.getTileSize().y};

        SDL_Rect* dest_rect = new SDL_Rect{ tile_position.x,
                                            tile_position.y,
                                            config.getTileSize().x,
                                            config.getTileSize().y};

        SDL_RenderCopy(sdl_renderer_, loadImage(tile->getTextureName()), src_rect, dest_rect);

        delete src_rect;
        delete dest_rect;
    }

    for(auto &tile : world_map[1])
    {
        if(tile != nullptr)
        {

            Point tile_position = tile->getPosition() * config.getTileSize() - camera_.getPosition();

            SDL_Rect* src_rect = new SDL_Rect{  0,
                                                0,
                                                config.getTileSize().x,
                                                config.getTileSize().y};

            SDL_Rect* dest_rect = new SDL_Rect{ tile_position.x,
                                                tile_position.y,
                                                config.getTileSize().x,
                                                config.getTileSize().y};

            SDL_RenderCopy(sdl_renderer_, loadImage(tile->getTextureName()), src_rect, dest_rect);

            delete src_rect;
            delete dest_rect;
        }
    }


    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);


    for(int x = 0 ; x < config.getMapSize().x ; x++)
    {
        SDL_RenderDrawLine(sdl_renderer_, x * config.getTileSize().x - camera_.getPosition().x, 0, x * config.getTileSize().x - camera_.getPosition().x, window_size_.y);
    }

    for(int y = 0 ; y < config.getMapSize().y ; y ++)
    {
        SDL_RenderDrawLine(sdl_renderer_, 0, y * config.getTileSize().y - camera_.getPosition().y, window_size_.x, y * config.getTileSize().y - camera_.getPosition().y);
    }

}

void Graphics::drawWindow(const Window* windo, const std::vector<Icon>& icon_list)
{
    Color color = windo->getColor();
    SDL_SetRenderDrawColor(sdl_renderer_, color.r, color.g, color.b, 0xFF);

    SDL_Rect* rect = new SDL_Rect{windo->getPosition().x, windo->getPosition().y, windo->getSize().x, windo->getSize().y};
    SDL_RenderFillRect(sdl_renderer_, rect);

    delete rect;


    for(auto &icon : icon_list)
    {
        Point icon_position = icon.getPosition();
        SDL_Rect* src_rect = new SDL_Rect{  0,
                                            0,
                                            icon.getSize().x,
                                            icon.getSize().y};

        SDL_Rect* dest_rect = new SDL_Rect{ icon_position.x,
                                            icon_position.y,
                                            icon.getSize().x,
                                            icon.getSize().y};

        SDL_RenderCopy(sdl_renderer_, loadImage(icon.getTextureName()), src_rect, dest_rect);


        if(&icon == windo->getSelectedIcon())
        {
            //SDL_Rect* selected_icon_rect = new SDL_Rect{}
            SDL_SetRenderDrawColor(sdl_renderer_, 255, 0, 0, 0xFF);
            SDL_RenderDrawRect(sdl_renderer_, dest_rect);
        }

        delete src_rect;
        delete dest_rect;

    }
}

void Graphics::drawNpcs(Config& config, std::vector<Peasant>& npcs_list, Peasant* selected_npc)
{


    for(auto &npc : npcs_list)
    {
        Point npc_position = npc.getPosition() * config.getTileSize() - camera_.getPosition();

        SDL_Rect* src_rect = new SDL_Rect{  0,
                                            0,
                                            config.getTileSize().x,
                                            config.getTileSize().y};

        SDL_Rect* dest_rect = new SDL_Rect{ npc_position.x,
                                            npc_position.y,
                                            config.getTileSize().x,
                                            config.getTileSize().y};

        SDL_RenderCopy(sdl_renderer_, loadImage(npc.getTextureName()), src_rect, dest_rect);


        if(&npc == selected_npc)
        {
            SDL_SetRenderDrawColor(sdl_renderer_, 150, 100, 10, 250);
            SDL_RenderDrawRect(sdl_renderer_, dest_rect);
        }
        delete src_rect;
        delete dest_rect;

        if(npc.getGoal() != npc.getPosition())
        {
            SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0x0F, 0x00, 0xFF);
            Point goal_position = npc.getGoal() * config.getTileSize() - camera_.getPosition();
            SDL_Rect* tile_rect = new SDL_Rect{ goal_position.x,
                                                goal_position.y,
                                                config.getTileSize().x,
                                                config.getTileSize().y};
            SDL_RenderFillRect(sdl_renderer_, tile_rect);

            delete tile_rect;
        }

    }


}



SDL_Texture* Graphics::loadImage(std::string file_name)
{
    if(sprite_sheets_.count(file_name) == 0)
    {
        SDL_Texture* t;
        t = IMG_LoadTexture(sdl_renderer_, file_name.c_str());
        if(t == nullptr)
        {
            printf("Cannot load texture called %s!", file_name.c_str());
            throw std::runtime_error("Cannot load texture called!");
        }

        sprite_sheets_[file_name] = t;
    }

    return sprite_sheets_[file_name];
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
