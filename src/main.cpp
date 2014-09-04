#include <SDL.h>
#include <iostream>
#include <stdio.h>

#include "main.h"
#include "Graphics.h"
#include "Coordinate.h"
#include "Game.h"
#include "time.h"
#include <SDL_ttf.h>
#define TAU 6.2831853071

#undef main



int main()
{
    srand ( time(NULL) );
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
    }
     TTF_Init();

    Game game_;

    std::cout << "Bye" << std::endl;

    TTF_Quit();
    SDL_Quit();

    return 0;
}
