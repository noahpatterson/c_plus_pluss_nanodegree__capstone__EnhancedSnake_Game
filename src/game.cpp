#include "game.h"
#include <iostream>
#include "SDL.h"
#include "button.h"
#include "Helpers.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height)
    : snake(grid_width, grid_height),
      _grid_height(grid_height),
      _grid_width(grid_width),
      _screen_height(screen_height),
      _screen_width(screen_width) {
  RandomPoint foodPoint(snake, grid_width, grid_height, _screen_width, _screen_height);
  PlaceFood(foodPoint.x, foodPoint.y);
}

void Game::Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  unsigned int donutTimer = 0;
  unsigned int bombTimer = 0;
  RandomPoint donutPoint(snake, _grid_width, _grid_height, _screen_width, _screen_height);
  RandomPoint bombPoint(snake, _grid_width, _grid_height, _screen_width, _screen_height);
  //create restart button
  Button restart_button;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);

    Update(donutPoint, bombPoint);
    renderer.Render(snake, food, restart_button, donutTimer, donutPoint, bombTimer, bombPoint);

    //extract this to method
    if (!snake.alive) {
      SDL_PumpEvents();
      int x, y;
      if (SDL_GetMouseState(&x, &y)) {
        bool inside = true;
        if (x < restart_button.position.x) {
          inside = false;
        } else if (x > restart_button.position.x + restart_button.relativeWidth) {
          inside = false;
        } else if (y < restart_button.position.y) {
          inside = false;
        } else if ( y > restart_button.position.y + restart_button.relativeHeight) {
          inside = false;
        }

        if (inside) {
          RestartGame();
        }
      }
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::RestartGame() {
  snake = Snake(_grid_width, _grid_height);
  snake.alive = true;
}

void Game::PlaceFood(int x, int y) {
  food.x = x;
  food.y = y;
}

void Game::Update(RandomPoint &donutPoint, RandomPoint &bombPoint) {
  if (!snake.alive) {
    return;
  };

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x / (_screen_width / _grid_width) == new_x && food.y / (_screen_height / _grid_height) == new_y) {
    score++;
    RandomPoint foodPoint(snake, _grid_width, _grid_height, _screen_width, _screen_height);
    PlaceFood(foodPoint.x, foodPoint.y);
    // Grow snake and increase speed.
    snake.GrowBody(1);
    snake.speed += 0.02;
  }

  // Check if snake eats donut
  int donutGridPointX =  donutPoint.x / (_screen_width / _grid_width);
  int donutGridPointY =  donutPoint.y / (_screen_height / _grid_height);
  if ((donutGridPointX == new_x && donutGridPointY == new_y) ||
      (donutGridPointX + 1 == new_x && donutGridPointY +1 == new_y) ||
      (donutGridPointX - 1 == new_x && donutGridPointY - 1 == new_y)) {
    score++;
    donutPoint.randomizePoint();
    // Grow snake and increase speed.
    snake.GrowBody(3);
    snake.speed += 0.02;
  }

  // Check if snake eats bomb
  int bombGridPointX =  bombPoint.x / (_screen_width / _grid_width);
  int bombGridPointY =  bombPoint.y / (_screen_height / _grid_height);
  if ((bombGridPointX == new_x && bombGridPointY == new_y) ||
      (bombGridPointX + 1 == new_x && bombGridPointY +1 == new_y) ||
      (bombGridPointX - 1 == new_x && bombGridPointY - 1 == new_y)) {
    snake.alive = false;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }