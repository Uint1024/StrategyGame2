#ifndef WINDOW_H
#define WINDOW_H

#include "Coordinate.h"
#include "Inputs.h"
#include "Icon.h"
class graphics;
class Window
{
    public:
        Window(Point position, Dimension size, bool visible, Color color);
        virtual ~Window();
        Point getPosition() const;
        Dimension getSize() const;
        bool isActive() const;
        bool isVisible() const;
        Color getColor() const;
        SDL_Rect getRect() const;
        void receiveInputs(Inputs& inputs);
        void render(Graphics& graphics);

    protected:
        Point position_;
        Dimension size_;
        bool active_;
        bool visible_;
        Color color_;
        SDL_Rect rect_;
        std::vector<Icon> icons_list_;
    private:

};

#endif // WINDOW_H
