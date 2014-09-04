#ifndef INPUTS_H
#define INPUTS_H
#include <SDL.h>
#include <iostream>

#include <map>
#include "Coordinate.h"
//#include "Window.h"
#include "Peasant.h"

class Window;
class Inputs
{
    public:
        Inputs();
        virtual ~Inputs();
        void keyDownEvent(const SDL_Event& event);
        void keyUpEvent(const SDL_Event& event);
        std::map<SDL_Scancode, bool> getPressedKeys() const;
        std::map<SDL_Scancode, bool> getHeldKeys() const;
        std::map<int, bool> getHeldMouseButtons() const;
        std::map<int, bool> getPressedMouseButtons() const;
        //std::map<SDL_Scancode, bool> getPressedKeysLastFrame() const;
        void mouseButtonDownEvent(const SDL_Event& event);
        void mouseButtonUpEvent(const SDL_Event& event);
        Point getMouseCoordinatesInPixels() const;
        Point getMouseTravel() const;
        void mouseMotionEvent(const SDL_Event& event);
        void mouseIsImmobile();
        Window* getLockedWindow() const;
        void calculateMouseTravel(const SDL_Event& event);
        void setLockedWindow(Window* windo);
        float getTimeSinceLastRightClick() const;
        void update(const SDL_Event& event);
        Point getMouseSelectionStart() const;
        Point getMouseSelectionEnd() const;

    protected:
    private:
        std::map<SDL_Scancode, bool> pressed_keys_;
        std::map<SDL_Scancode, bool> pressed_keys_last_frame;
        std::map<SDL_Scancode, bool> held_keys_;
        std::map<int, bool> pressed_mouse_buttons_;
        std::map<int, bool> held_mouse_buttons; //held for more than 1 frame
        Point mouse_coordinates_in_pixels;
        Point mouse_travel_; //to calculate mouse travel between frames
        Window* cursor_locked_on_;
        Peasant* selected_npc_;
        float last_right_click_;

        Point mouse_selection_start;
        Point mouse_selection_end;

        int left_click_duration_;
        int left_click_start_;
        bool left_click_started_;
        bool selection_has_started_;
};

#endif // INPUTS_H
