#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include "Helpers.h"
#include "button.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}

bool Controller::CheckButtonInteraction(Button &button, int &x, int &y) const {
  bool inside = true;
  if (x < button.position.x) {
    inside = false;
  } else if (x > button.position.x + button.relativeWidth) {
    inside = false;
  } else if (y < button.position.y) {
    inside = false;
  } else if ( y > button.position.y + button.relativeHeight) {
    inside = false;
  }
  return inside;
}

MouseActionButtons Controller::HandleMouseLocation(Button &restart_button, Button &score_button, Button &saveScoreButton) const {
  int x, y;
  if (SDL_GetMouseState(&x, &y)) {
    if (CheckButtonInteraction(restart_button, x, y)) {
      return MouseActionButtons::restart;
    }

    if (CheckButtonInteraction(score_button, x, y)) {
      return MouseActionButtons::score;
    }

    if (CheckButtonInteraction(saveScoreButton, x, y)) {
      return MouseActionButtons::save_score;
    }
  }
  return MouseActionButtons::none;
}