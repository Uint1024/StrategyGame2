#include "Inputs.h"

Inputs::Inputs() : cursor_locked_on_(nullptr)
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
    //mouse_travel_ = Point{event.button.x, event.button.y} - mouse_coordinates_in_pixels;
    pressed_mouse_buttons_[event.button.button] = true;

}

void Inputs::mouseButtonUpEvent(const SDL_Event& event)
{
    pressed_mouse_buttons_[event.button.button] = false;
    std::cout << "released key " << event.button.button << std::endl;
    cursor_locked_on_ = nullptr;
}

void Inputs::mouseMotionEvent(const SDL_Event& event)
{
   // mouse_travel_ = Point{event.button.x, event.button.y} - mouse_coordinates_in_pixels;

    //mouse_coordinates_in_pixels = {event.motion.x, event.motion.y};
    //mouse_coordinates_in_pixels = {event.motion.x, event.motion.y};
}

void Inputs::mouseIsImmobile() { mouse_travel_ = Point{0,0};}

void Inputs::calculateMouseTravel(const SDL_Event& event)
{
    mouse_travel_ = Point{event.button.x, event.button.y} - mouse_coordinates_in_pixels;
    mouse_coordinates_in_pixels = {event.motion.x, event.motion.y};

}

Window* Inputs::getLockedWindow() const { return cursor_locked_on_; }
void Inputs::setLockedWindow(Window* windo) { cursor_locked_on_ = windo; std::cout << "wow" << std::endl; }
Point Inputs::getMouseCoordinatesInPixels() const { return mouse_coordinates_in_pixels; }
std::map<SDL_Scancode, bool> Inputs::getPressedKeys() const { return pressed_keys_; }
std::map<int, bool> Inputs::getPressedMouseButtons() const { return pressed_mouse_buttons_; }
Point Inputs::getMouseTravel() const { return mouse_travel_; }
