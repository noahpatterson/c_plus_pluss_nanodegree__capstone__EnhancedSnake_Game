#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL2/SDL.h"
// #include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include "texture.h"


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

  private:
    SDL_Rect _spriteClip;
    Clip _clipType;
};

#endif