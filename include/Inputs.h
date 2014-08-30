#ifndef INPUTS_H
#define INPUTS_H
#include <SDL.h>
#include <iostream>

#include <map>
#include "Coordinate.h"

class Inputs
{
    public:
        Inputs();
        virtual ~Inputs();
        void keyDownEvent(const SDL_Event& event);
        void keyUpEvent(const SDL_Event& event);
        std::map<SDL_Scancode, bool> getPressedKeys();
        std::map<int, bool> getPressedMouseButtons();

        void mouseButtonDownEvent(const SDL_Event& event);
        void mouseButtonUpEvent(const SDL_Event& event);
        Point getMouseCoordinatesInPixels();
        void mouseMotionEvent(const SDL_Event& event);

    protected:
    private:
        std::map<SDL_Scancode, bool> pressed_keys_;
        std::map<int, bool> pressed_mouse_buttons_;
        Point mouse_coordinates_in_pixels;
        //Point mouse_coordinates_in_tiles;
};

#endif // INPUTS_H
