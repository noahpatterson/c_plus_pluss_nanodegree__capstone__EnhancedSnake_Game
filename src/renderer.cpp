#include "renderer.h"
#include "button.h"
#include <iostream>
#include <string>
#include "texture.h"
#include "obstacle.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "Helpers.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  TTF_CloseFont(font);
  font = NULL;
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  sdl_window = NULL;
  sdl_renderer = NULL;
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food, Button &restart_button, Button &score_button, Button &saveScoreButton, unsigned int &donutTimer, RandomPoint &donutPoint, unsigned int &bombTimer, RandomPoint &bombPoint) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x;
  block.y = food.y;
  SDL_RenderFillRect(sdl_renderer, &block);

  //render donut item
  Obstacle donut(sdl_renderer, Clip::donut);
  //Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags))
  {
    printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
  } else {
    RenderObstacle(donut, donutPoint, donutTimer, snake);
  }
  //render bomb item
  Obstacle bomb(sdl_renderer, Clip::bomb);
  RenderObstacle(bomb, bombPoint, bombTimer, snake);

  // Render snake's body
  if (snake.growing) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x7F, 0xBF, 0xFF, 0x3F);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else if (snake.growing) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x7F, 0xBF, 0xFF, 0x3F);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xF3, 0x1B, 0x1B, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  //render restart and show score buttons if snake is dead.
  if (!snake.alive) {
    bool success = true;
    success = InitFont();

    if (success) {
      RenderButton(restart_button, "Restart Game", 100);
      RenderButton(score_button, "Show Score", -100);
    }
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderScore(int score, int size, Snake const &snake, Button &restart_button, Button &saveScoreButton) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(sdl_renderer);

  //setup texture
  bool success = true;
  success = InitFont();

  if (success) {
    RenderText("Score: " + std::to_string(score), -100, 100);
    RenderText("Score: " + std::to_string(size), 100, 100);
    RenderButton(restart_button, "Restart Game", 100);
    RenderButton(saveScoreButton, "Save Score", 100, -100);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderUserInput(std::string text, Button &saveScoreWithCustomFileButton, int &timer, bool &errorInFile) {
  //render question for input
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(sdl_renderer);

  if (errorInFile) {
    SDL_Color color = {237, 27, 12};
    RenderText("The filename entered is not valid. Path should start with a '.' or a '/' and end with a '/'.", 0, 200, color);
    }

    RenderText("Please enter a file path:");

    if (timer < 60) {
      RenderText("File Path: " + text, 0, -50);
      ++timer;
    } else {
      RenderText("File Path: " + text + "|", 0, -50);
      ++timer;
      if (timer > 90) {
        timer = 0;
      }
    }

    RenderButton(saveScoreWithCustomFileButton, "Save Score", 0, -100);

    // Update Screen
    SDL_RenderPresent(sdl_renderer);
  }

  void Renderer::UpdateWindowTitle(int score, int fps) {
    std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
  }

  void Renderer::RenderButton(Button &button, std::string buttonText, int xPositionOffset, int yPositionOffset) {
    //setup texture
    Texture texture(sdl_renderer);
    bool success = true;
    //Render text
    SDL_Color textColor = { 0, 0, 0 };
    if(!texture.loadFromRenderedText( buttonText, textColor, font )) {
        printf("Failed to render text texture!\n");
        success = false;
    }
    int textureWidth  = texture.getWidth();
    int textureHeight = texture.getHeight();
    int buttonPositionX = (screen_width - textureWidth) / 2 - xPositionOffset;
    int buttonPositionY = (screen_height -  textureHeight) / 2 - yPositionOffset;

    button.setRelativeHeight(textureHeight, 0, true);
    button.setRelativeWidth(textureWidth, 0, true);
    button.position.x = buttonPositionX;
    button.position.y = buttonPositionY;
    SDL_SetRenderDrawColor(sdl_renderer, 0xEF, 0xF2, 0x49, 0xFF); //#EFF249
    SDL_Rect buttonRect = button.createButtonRect();
    SDL_RenderFillRect(sdl_renderer, &buttonRect);

    if (success) {
      texture.render(buttonPositionX, buttonPositionY);
    }
  }

  void Renderer::RenderObstacle(Obstacle &obstacle, RandomPoint &point, unsigned int &timer, const Snake &snake) {
    const unsigned int timeTillReset = 600;
    if (timer < timeTillReset && snake.alive) {
      obstacle.render(point.x, point.y);
      ++timer;
    } else {
      //create a random point to move the donut to
      point.randomizePoint();
      timer = 0;
    }
  }

  void Renderer::RenderText(std::string text, int xPositionOffset, int yPositionOffset, SDL_Color textColor) {
    //show size
    Texture texture(sdl_renderer);



  bool success = true;
  // std::string _size(text + std::to_string(sizeOrScore));
  if(!texture.loadFromRenderedText(text, textColor, font )) {
      printf("Failed to render text texture!\n");
      success = false;
  }

  int textureWidth  = texture.getWidth();
  int textureHeight = texture.getHeight();

  if (success) {
    texture.render((screen_width - textureWidth) / 2 - xPositionOffset, (screen_height -  textureHeight) / 2 - yPositionOffset);
  }
}

bool Renderer::InitFont() {
  bool success = true;
  if (!TTF_isInit) {
    //Initialize SDL_ttf
    if(TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    } else {
      TTF_isInit = true;
    }
  }

  if (!fontInit) {
    font = TTF_OpenFont("../fonts/Roboto-Black.ttf", 28);
    if(font == NULL) {
      printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
      success = false;
    } else {
      fontInit = true;
    }
  }
  return success;
}
