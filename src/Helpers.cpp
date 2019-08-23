#include <random>
#include "snake.h"
#include "Helpers.h"

RandomPoint::RandomPoint(Snake &snake, std::size_t grid_width, std::size_t grid_height, std::size_t screen_width,
std::size_t screen_height)
  : _snake(snake),
     engine(dev()),
     random_w(0, static_cast<int>(grid_width)),
     random_h(0, static_cast<int>(grid_height)),
     _grid_width(grid_width),
     _grid_height(grid_height),
     _screen_width(screen_width),
     _screen_height(screen_height) {
    randomizePoint();
  }

void RandomPoint::randomizePoint() {
  int random_x, random_y;
  while (true) {
    random_x = random_w(engine);
    random_y = random_h(engine);

    if (!_snake.SnakeCell(random_x, random_y)) {
      int trueX = random_x * (_screen_width / _grid_width);
      int trueY = random_y * (_screen_height / _grid_height);
      if (trueX < 50) {
        x = trueX + 25;
      } else if (trueX == 640) {
        x = trueX - 25;
      } else {
        x = trueX;
      }

      if (trueY < 50) {
        y = trueY + 25;
      } else if (trueY == 640) {
        y = trueY - 25;
      } else {
        y = trueY;
      }
      return;
    }
  }
}