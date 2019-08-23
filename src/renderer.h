#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "button.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "snake.h"
#include "Helpers.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food, Button &restart_button, unsigned int &donutTimer, RandomPoint &donutPoint, unsigned int &bombTimer, RandomPoint &bombPoint);

  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  TTF_Font *font;
};

#endif