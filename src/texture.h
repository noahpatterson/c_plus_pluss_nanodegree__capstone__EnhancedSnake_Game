// credit to http://www.lazyfoo.net/tutorials/SDL/10_color_keying/index.php
// http://www.lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php

#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>

class Texture
{
    public:
        //Initializes variables
        Texture(SDL_Renderer *sdl_renderer)
          : _sdl_renderer(sdl_renderer) {
              //Initialize
              _texture = NULL;
              _width = 0;
              _height = 0;
          };

        //Deallocates memory
        ~Texture();

        //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font *font );

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render( int x, int y);

        int getWidth();
        int getHeight();
        SDL_Texture* getTexture();
        SDL_Renderer* getSDLRenderer();

        void setWidth(int width);
        void setHeight(int height);
        void setTexture(SDL_Texture *t);
        void setSDLRenderer(SDL_Renderer *sdlr);

    private:
        //The actual hardware texture
        SDL_Texture *_texture;

        //Image dimensions
        int _width;
        int _height;

        SDL_Renderer *_sdl_renderer;
};

#endif