#include "button.h"
#include <iostream>

SDL_Rect Button::createButtonRect() {
  SDL_Rect block;
  block.x = _position.x;
  block.y = _position.y;
  block.w = _relativeWidth;
  block.h = _relativeHeight;
  return block;
}

void Button::setRelativeWidth(const std::size_t  screenWidth, const std::size_t  gridWidth) {
  _relativeWidth = (screenWidth / gridWidth) * _width;
}
void Button::setRelativeHeight(const std::size_t  screenHeight, const std::size_t  gridHeight){
  _relativeHeight = (screenHeight/ gridHeight) * _height;
}