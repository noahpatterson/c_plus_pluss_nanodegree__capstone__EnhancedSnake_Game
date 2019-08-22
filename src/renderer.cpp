#include "renderer.h"
#include "button.h"
#include <iostream>
#include <string>
#include "texture.h"
#include "obstacle.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

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
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food, Button &restart_button) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render restart button
  if (!snake.alive) {
    //setup texture
    Texture textTexture(sdl_renderer);
    bool success = true;
    //Initialize SDL_ttf
    if(TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    font = TTF_OpenFont("../fonts/Roboto-Black.ttf", 28);
    if(font == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    } else {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };
        if(!textTexture.loadFromRenderedText( "Restart Game", textColor, font )) {
            printf("Failed to render text texture!\n");
            success = false;
        }
    }
    int textureWidth  = textTexture.getWidth();
    int textureHeight = textTexture.getHeight();
    int buttonPositionX = (screen_width - textureWidth) / 2;
    int buttonPositionY = (screen_height -  textureHeight) / 2;

    restart_button.setRelativeHeight(textureHeight, 0, true);
    restart_button.setRelativeWidth(textureWidth, 0, true);
    restart_button.position.x = buttonPositionX;
    restart_button.position.y = buttonPositionY;
    SDL_SetRenderDrawColor(sdl_renderer, 0xEF, 0xF2, 0x49, 0xFF); //#EFF249
    SDL_Rect restartButton = restart_button.createButtonRect();
    SDL_RenderFillRect(sdl_renderer, &restartButton);

    //render text on top of button
    if (success) {
      textTexture.render(buttonPositionX, buttonPositionY);
    }
  }

  //render donut item
  Obstacle donut(sdl_renderer, Clip::donut);

  //Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags))
  {
    printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
  } else {
    donut.render(0, 0);
  }


  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
