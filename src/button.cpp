#include "button.h"
#include <iostream>
#include <stdexcept>

Button::Button() {}

SDL_Rect Button::createButtonRect() {
  SDL_Rect block;
  block.x = position.x;
  block.y = position.y;
  block.w = relativeWidth;
  block.h = relativeHeight;
  return block;
}

void Button::setRelativeWidth(const std::size_t  screenWidth, const std::size_t gridWidth, bool copyChildSize) {
  if (gridWidth != 0) {
    // place empty button
    relativeWidth = (screenWidth / gridWidth) * width;
  } else if (copyChildSize) {
    relativeWidth = screenWidth;
  } else {
    throw std::invalid_argument("Must also include gridWidth or copyChildSize");
  }
}
void Button::setRelativeHeight(const std::size_t  screenHeight, const std::size_t gridHeight, bool copyChildSize){
  if (gridHeight != 0) {
    // place empty button
    relativeHeight = (screenHeight/ gridHeight) * height;
  } else if (copyChildSize) {
    relativeHeight = screenHeight;
  } else {
    throw std::invalid_argument("Must also include gridHeight or copyChildSize");
  }
}