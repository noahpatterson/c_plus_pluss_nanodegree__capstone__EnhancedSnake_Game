#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <string>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height);
  void Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void RestartGame();

 private:
  Snake snake;
  SDL_Point food;

  int score{0};
  bool showingScore{false};
  bool renderInput{false};
  bool hasSavedFile{false};
  bool errorInFile{false};
  std::string inputText{""};

  void PlaceFood(int x, int y);
  void Update(RandomPoint &donutPoint, RandomPoint &bombPoint);
  void SaveScoreToFile(int score, int size);
  int _grid_width;
  int _grid_height;
  int _screen_width;
  int _screen_height;
};

#endif