#include "Inputs.h"

Inputs::Inputs() : cursor_locked_on_(nullptr)
{
    //ctor
    left_click_duration_ = 0;
    left_click_start_ = 0;
    left_click_started_ = false;
}

Inputs::~Inputs()
{
    //dtor
}


void Inputs::keyDownEvent(const SDL_Event& event)
{
    if(pressed_keys_[event.key.keysym.scancode] == true)
    {
        pressed_keys_[event.key.keysym.scancode] = false;
        held_keys_[event.key.keysym.scancode] = true;
    }
    else
    {
        pressed_keys_[event.key.keysym.scancode] = true;
        held_keys_[event.key.keysym.scancode] = false;
    }

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


    left_click_start_ = SDL_GetTicks();



}

void Inputs::update()
{
    if(pressed_mouse_buttons_[SDL_BUTTON_LEFT])
        left_click_duration_ = SDL_GetTicks() - left_click_start_;

    if(left_click_duration_ > 100)
    {
        held_mouse_buttons[SDL_BUTTON_LEFT] = true;
        if(selection_has_started_ == false)
        {
            mouse_selection_start = mouse_coordinates_in_pixels;
        }
        else
        {
            mouse_selection_end = mouse_coordinates_in_pixels;
        }
    }
}

void Inputs::mouseMotionEvent(const SDL_Event& event) {}
void Inputs::mouseButtonUpEvent(const SDL_Event& event)
{
    pressed_mouse_buttons_[event.button.button] = false;
    held_mouse_buttons[event.button.button] = false;
    cursor_locked_on_ = nullptr;


    left_click_duration_ = 0;
    left_click_started_ = false;
    selection_has_started_ = false;
}

float Inputs::getTimeSinceLastRightClick() const
{
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
std::map<int, bool> Inputs::getHeldMouseButtons() const { return held_mouse_buttons; }
Point Inputs::getMouseTravel() const { return mouse_travel_; }
std::map<SDL_Scancode, bool> Inputs::getHeldKeys() const { return held_keys_; }
