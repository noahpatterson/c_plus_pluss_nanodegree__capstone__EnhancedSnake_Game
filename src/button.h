#ifndef BUTTON_H
#define BUTTON_H

#include "SDL.h"

// used https://lazyfoo.net/tutorials/SDL/17_mouse_events/index.php
class Button {
  public:
    Button(int x, int y, int width, int height)
        : _width(width), _height(height) {
        _position.x = x;
        _position.y = y;
      };
    SDL_Rect createButtonRect();
    void setRelativeWidth(const std::size_t  screenWidth, const std::size_t  gridWidth);
    void setRelativeHeight(const std::size_t  screenHeight, const std::size_t  gridHeight);
    SDL_Point _position;
    int _relativeWidth;
    int _relativeHeight;


  private:
    int _width;
    int _height;
};

#endif
