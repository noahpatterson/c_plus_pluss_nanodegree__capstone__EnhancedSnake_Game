#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "button.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "snake.h"
#include "Helpers.h"
#include "obstacle.h"
#include <string>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food, Button &restart_button, Button &score_button, Button &saveScoreButton, unsigned int &donutTimer, RandomPoint &donutPoint, unsigned int &bombTimer, RandomPoint &bombPoint);

  void RenderScore(int score, int size, Snake const &snake, Button &restart_button, Button &saveScoreButton);

  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  TTF_Font *font;
  bool TTF_isInit{false};
  bool fontInit{false};

  void RenderButton(Button &button, std::string buttonText, int xPositionOffset = 0, int yPositionOffset = 0);
  void RenderObstacle(Obstacle &obstacle, RandomPoint &point, unsigned int &timer, const Snake &snake);
  void RenderText(std::string text, int sizeOrScore, int xPositionOffset = 0, int yPositionOffset = 0);
  bool InitFont();
};

#endif