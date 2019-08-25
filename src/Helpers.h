#ifndef HELPERS_H
#define HELPERS_H

#include <random>
#include "snake.h"

enum MouseActionButtons {
  restart,
  score,
  save_score,
  none
};

struct RandomPoint {
  public:
    RandomPoint(Snake &snake, std::size_t grid_width, std::size_t grid_height, std::size_t screen_width,
    std::size_t screen_height);
    int x;
    int y;

    void randomizePoint();

  private:
    Snake _snake;
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
    std::size_t _grid_width;
    std::size_t _grid_height;
    std::size_t _screen_width;
    std::size_t _screen_height;

};

void writeScoreFile(int score, int size);

#endif