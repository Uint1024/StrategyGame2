#include "Inputs.h"

Inputs::Inputs()
{
    //ctor
}

Inputs::~Inputs()
{
    //dtor
}


void Inputs::keyDownEvent(const SDL_Event& event)
{
    pressed_keys_[event.key.keysym.scancode] = true;
}

void Inputs::keyUpEvent(const SDL_Event& event)
{
    pressed_keys_[event.key.keysym.scancode] = false;
}

void Inputs::mouseButtonDownEvent(const SDL_Event& event)
{
    pressed_mouse_buttons_[event.button.button] = true;

    mouse_coordinates_in_pixels = {event.button.x, event.button.y};
    //mouse_coordinates_in_pixels = {event.button.x, event.button.y};
}

void Inputs::mouseButtonUpEvent(const SDL_Event& event)
{
    pressed_mouse_buttons_[event.button.button] = false;
    std::cout << "released key " << event.button.button << std::endl;
}

void Inputs::mouseMotionEvent(const SDL_Event& event)
{
    mouse_coordinates_in_pixels = {event.motion.x, event.motion.y};
}

Point Inputs::getMouseCoordinatesInPixels() { return mouse_coordinates_in_pixels; }
std::map<SDL_Scancode, bool> Inputs::getPressedKeys(){ return pressed_keys_; }
std::map<int, bool> Inputs::getPressedMouseButtons() { return pressed_mouse_buttons_; }
