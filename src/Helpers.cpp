#include <random>
#include "snake.h"
#include "Helpers.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <string>

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

void writeScoreFile(int score, int size, std::string &fileLocation, bool &errorInFile) {
  std::ofstream scoreFile;

  //check if user entry is a filepath
  std::regex regx("^\\.+\\/*.*\\/+$");
  if (fileLocation.empty()) {
    fileLocation = "";
  } else if (!std::regex_match(fileLocation.begin(), fileLocation.end(), regx)) {
    errorInFile = true;
    return;
  }

  scoreFile.open(fileLocation + "SnakeGame_Scores.txt", std::ios::app);
  if (scoreFile) {
    scoreFile << "Score: " << score << " Size: " << size << "\n";
    scoreFile.close();
    errorInFile = false;
  }
}