#include "game.h"
#include <iostream>
#include "SDL.h"
#include "button.h"
#include "Helpers.h"
#include "obstacle.h"
#include <string>

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
  int inputTimer = 0;
  RandomPoint donutPoint(snake, _grid_width, _grid_height, _screen_width, _screen_height);
  RandomPoint bombPoint(snake, _grid_width, _grid_height, _screen_width, _screen_height);
  //create restart button
  Button restart_button;
  Button score_button;
  Button saveScoreButton;
  Button saveScoreWithCustomFileButton;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    SDL_StartTextInput();
    controller.HandleInput(running, snake, inputText, hasSavedFile);


    if (!showingScore) {
      Update(donutPoint, bombPoint);
      renderer.Render(snake, food, restart_button, score_button, saveScoreButton, donutTimer, donutPoint, bombTimer, bombPoint);
    }

    if (showingScore && renderInput) {
      renderer.RenderUserInput(inputText, saveScoreWithCustomFileButton, inputTimer, errorInFile);
    }

    if (!snake.alive) {
      MouseActionButtons buttonState = controller.HandleMouseLocation(restart_button, score_button, saveScoreButton, saveScoreWithCustomFileButton);
      switch (buttonState) {
        case MouseActionButtons::restart: {
          showingScore = false;
          RestartGame();
          break;
        }
        case MouseActionButtons::score: {
          showingScore = true;
          renderer.RenderScore(GetScore(), GetSize(), snake, restart_button, saveScoreButton);
          break;
        }
        case MouseActionButtons::save_score: {
          renderInput = true;
          hasSavedFile = false;
          break;
        }
        case MouseActionButtons::saveScoreWithCustomFile: {
          if (!hasSavedFile) {
            writeScoreFile(GetScore(), GetSize(), inputText, errorInFile);
            if (!errorInFile) {
              hasSavedFile = true;
              showingScore = false;
              renderInput = false;
            }
          }
          break;
        }
        case MouseActionButtons::none: {
        }
        default: {
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
  score = 0;
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

  if (Obstacle::checkCollision(donutPoint, _screen_width, _screen_height, _grid_width, _grid_height, new_x, new_y)) {
    score+= 10;
    donutPoint.randomizePoint();
    // Grow snake and increase speed.
    snake.GrowBody(3);
    snake.speed += 0.02;
  }

  if (Obstacle::checkCollision(bombPoint, _screen_width, _screen_height, _grid_width, _grid_height, new_x, new_y)) {
    snake.alive = false;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }