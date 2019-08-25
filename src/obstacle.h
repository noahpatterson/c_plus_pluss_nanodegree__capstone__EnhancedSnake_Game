#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL2/SDL.h"
// #include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include "texture.h"
#include "Helpers.h"


enum Clip {
  donut,
  bomb
};

class Obstacle : public Texture {
  public:
    Obstacle(SDL_Renderer *sdl_renderer, Clip c);

    // ~Obstacle();

    //Loads image at specified path
    bool loadFromFile(std::string path);

    void render(int x, int y);

    bool loadMedia();
    static bool checkCollision(RandomPoint &point, int &screen_width, int &screen_height, int &grid_width, int &grid_height, int &new_x, int &new_y);

  private:
    SDL_Rect _spriteClip;
    Clip _clipType;
};

#endif