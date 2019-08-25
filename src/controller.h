#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "Helpers.h"
#include "button.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake, std::string &inputText, bool &hasSavedFile) const;
  MouseActionButtons HandleMouseLocation(Button &restart_button, Button &score_button, Button &saveScoreButton, Button &saveScoreWithCustomFileButton) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  bool CheckButtonInteraction(Button &button, int &x, int &y) const;
};

#endif