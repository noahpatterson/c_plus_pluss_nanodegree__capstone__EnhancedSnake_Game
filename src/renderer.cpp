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
  //move donut every 10 seconds (when running at 60fps)
  const unsigned int timeTillDonutReset = 600;
  //Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags))
  {
    printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
  } else if (donutTimer < timeTillDonutReset && snake.alive) {
    donut.render(donutPoint.x, donutPoint.y);
    ++donutTimer;
  } else {
    //create a random point to move the donut to
    donutPoint.randomizePoint();
    donutTimer = 0;
  }

  //render bomb item
  Obstacle bomb(sdl_renderer, Clip::bomb);
  //move donut every 10 seconds (when running at 60fps)
  const unsigned int timeTillBombReset = 600;
  if (bombTimer < timeTillBombReset && snake.alive) {
    bomb.render(bombPoint.x, bombPoint.y);
    ++bombTimer;
  } else {
    //create a random point to move the donut to
    bombPoint.randomizePoint();
    bombTimer = 0;
  }

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

  //setup show Score button
  //setup texture
  if (!snake.alive) {
    bool success = true;
    success = InitFont();
    Texture showScoreTexture(sdl_renderer);
    SDL_Color textColor = { 0, 0, 0 };
    if (success) {
      //Render text
      if(!showScoreTexture.loadFromRenderedText( "Show Score", textColor, font )) {
          printf("Failed to render text texture!\n");
          success = false;
      }
    }

    int textureWidth  = showScoreTexture.getWidth();
    int textureHeight = showScoreTexture.getHeight();
    int scoreButtonPositionX = (screen_width - textureWidth) / 2 + 100;
    int scoreButtonPositionY = (screen_height -  textureHeight) / 2;

    score_button.setRelativeHeight(textureHeight, 0, true);
    score_button.setRelativeWidth(textureWidth, 0, true);
    score_button.position.x = scoreButtonPositionX;
    score_button.position.y = scoreButtonPositionY;
    SDL_SetRenderDrawColor(sdl_renderer, 0xEF, 0xF2, 0x49, 0xFF); //#EFF249
    SDL_Rect scoreButton = score_button.createButtonRect();
    SDL_RenderFillRect(sdl_renderer, &scoreButton);


    //render text on top of button
    if (success) {
      RenderRestartButton(snake, restart_button);
      showScoreTexture.render(scoreButtonPositionX, scoreButtonPositionY);
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
  Texture scoreTexture(sdl_renderer);
  if (success) {
    //Render text
    SDL_Color textColor = { 0, 0, 0 };
    std::string _score("Score: " + std::to_string(score));
    if(!scoreTexture.loadFromRenderedText( _score, textColor, font )) {
        printf("Failed to render text texture!\n");
        success = false;
    }
  }

  int textureWidth  = scoreTexture.getWidth();
  int textureHeight = scoreTexture.getHeight();

  //show size
  Texture sizeTexture(sdl_renderer);
  if (success) {
    SDL_Color textColor = { 0, 0, 0 };
    std::string _size("Size: " + std::to_string(size));
    if(!sizeTexture.loadFromRenderedText( _size, textColor, font )) {
        printf("Failed to render text texture!\n");
        success = false;
    }
  }

  int sizeTextureWidth  = sizeTexture.getWidth();
  int sizeTextureHeight = sizeTexture.getHeight();

  if (success) {
    scoreTexture.render((screen_width - textureWidth) / 2 + 100, (screen_height -  textureHeight) / 2 - 100);
    sizeTexture.render((screen_width - textureWidth) / 2 - 100, (screen_height -  textureHeight) / 2 - 100);
    RenderRestartButton(snake, restart_button);
    RenderSaveScoreButton(snake, saveScoreButton);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::RenderRestartButton(Snake const &snake, Button &restart_button) {
  // Render restart button

  //setup texture
  Texture restartTexture(sdl_renderer);
  bool success = true;
  //Render text
  SDL_Color textColor = { 0, 0, 0 };
  if(!restartTexture.loadFromRenderedText( "Restart Game", textColor, font )) {
      printf("Failed to render text texture!\n");
      success = false;
  }
  int textureWidth  = restartTexture.getWidth();
  int textureHeight = restartTexture.getHeight();
  int restartButtonPositionX = (screen_width - textureWidth) / 2 - 100;
  int restartButtonPositionY = (screen_height -  textureHeight) / 2;

  restart_button.setRelativeHeight(textureHeight, 0, true);
  restart_button.setRelativeWidth(textureWidth, 0, true);
  restart_button.position.x = restartButtonPositionX;
  restart_button.position.y = restartButtonPositionY;
  SDL_SetRenderDrawColor(sdl_renderer, 0xEF, 0xF2, 0x49, 0xFF); //#EFF249
  SDL_Rect restartButton = restart_button.createButtonRect();
  SDL_RenderFillRect(sdl_renderer, &restartButton);

  if (success) {
    restartTexture.render(restartButtonPositionX, restartButtonPositionY);
  }
}

void Renderer::RenderSaveScoreButton(Snake const &snake, Button &saveScoreButton) {
  // Render restart button

  //setup texture
  Texture saveScoreTexture(sdl_renderer);
  bool success = true;
  //Render text
  SDL_Color textColor = { 0, 0, 0 };
  if(!saveScoreTexture.loadFromRenderedText( "Save Score", textColor, font )) {
      printf("Failed to render text texture!\n");
      success = false;
  }
  int textureWidth  = saveScoreTexture.getWidth();
  int textureHeight = saveScoreTexture.getHeight();
  int saveScoreButtonPositionX = (screen_width - textureWidth) / 2 - 100;
  int saveScoreButtonPositionY = (screen_height -  textureHeight) / 2 + 100;

  saveScoreButton.setRelativeHeight(textureHeight, 0, true);
  saveScoreButton.setRelativeWidth(textureWidth, 0, true);
  saveScoreButton.position.x = saveScoreButtonPositionX;
  saveScoreButton.position.y = saveScoreButtonPositionY;
  SDL_SetRenderDrawColor(sdl_renderer, 0xEF, 0xF2, 0x49, 0xFF); //#EFF249
  SDL_Rect saveScoreButtonRect = saveScoreButton.createButtonRect();
  SDL_RenderFillRect(sdl_renderer, &saveScoreButtonRect);

  if (success) {
    saveScoreTexture.render(saveScoreButtonPositionX, saveScoreButtonPositionY);
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
