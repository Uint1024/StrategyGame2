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
    if(event.button.button == SDL_BUTTON_RIGHT)
        last_right_click_ = SDL_GetTicks();

    pressed_mouse_buttons_[event.button.button] = true;

}

void Inputs::mouseMotionEvent(const SDL_Event& event) {}
void Inputs::mouseButtonUpEvent(const SDL_Event& event)
{
    pressed_mouse_buttons_[event.button.button] = false;
    held_mouse_buttons[event.button.button] = false;
    cursor_locked_on_ = nullptr;
}

float Inputs::getTimeSinceLastRightClick() const
{
    //std::cout << SDL_GetTicks() - last_right_click_ << std::endl;
    return (SDL_GetTicks() - last_right_click_);
}

void Inputs::mouseIsImmobile() { mouse_travel_ = Point{0,0};}

void Inputs::calculateMouseTravel(const SDL_Event& event)
{
    mouse_travel_ = Point{event.button.x, event.button.y} - mouse_coordinates_in_pixels;
    mouse_coordinates_in_pixels = {event.motion.x, event.motion.y};

}

Window* Inputs::getLockedWindow() const { return cursor_locked_on_; }
void Inputs::setLockedWindow(Window* windo) { cursor_locked_on_ = windo;}
Point Inputs::getMouseCoordinatesInPixels() const { return mouse_coordinates_in_pixels; }
std::map<SDL_Scancode, bool> Inputs::getPressedKeys() const { return pressed_keys_; }
std::map<int, bool> Inputs::getPressedMouseButtons() const { return pressed_mouse_buttons_; }
Point Inputs::getMouseTravel() const { return mouse_travel_; }
