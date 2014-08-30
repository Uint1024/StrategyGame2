#include <SDL.h>
#include <iostream>
#include <stdio.h>

#include "main.h"
#include "Graphics.h"
#include "Coordinate.h"
#include "Game.h"

#define TAU 6.2831853071

#undef main



int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
    }

    Game game_;

    std::cout << "Bye" << std::endl;

    return 0;
}
