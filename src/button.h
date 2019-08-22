#ifndef BUTTON_H
#define BUTTON_H

#include "SDL.h"

// used https://lazyfoo.net/tutorials/SDL/17_mouse_events/index.php
class Button {
  public:
    Button();

    SDL_Rect createButtonRect();
    void setRelativeWidth(const std::size_t screenWidth, const std::size_t gridWidth = 0, bool copyChildSize = false);
    void setRelativeHeight(const std::size_t screenHeight, const std::size_t gridHeight = 0, bool copyChildSize = false);
    SDL_Point position;
    int relativeWidth;
    int relativeHeight;
    int width;
    int height;
};

#endif
