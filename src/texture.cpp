// credit to http://www.lazyfoo.net/tutorials/SDL/10_color_keying/index.php
// http://www.lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php

#include "SDL2/SDL.h"
#include "texture.h"
#include "SDL2/SDL_ttf.h"
// #include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>

Texture::~Texture()
{
    //Deallocate
    free();
}

bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font *font )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    //need to pass in font
    SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        _texture = SDL_CreateTextureFromSurface( _sdl_renderer, textSurface );
        if( _texture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            _width = textSurface->w;
            _height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return _texture != NULL;
}

void Texture::free()
{
    //Free texture if it exists
    if( _texture != NULL )
    {
        SDL_DestroyTexture( _texture );
        _texture = NULL;
        _width = 0;
        _height = 0;
    }
}

void Texture::render(int x, int y)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, _width, _height };

    //Render to screen
    SDL_RenderCopy(_sdl_renderer, _texture, NULL, &renderQuad);
}

int Texture::getWidth()
{
    return _width;
}

int Texture::getHeight()
{
    return _height;
}

SDL_Texture* Texture::getTexture() {
    return _texture;
}
SDL_Renderer* Texture::getSDLRenderer() {
    return _sdl_renderer;
}

void Texture::setWidth(int width) {
    _width = width;
}
void Texture::setHeight(int height) {
    _height = height;
}
void Texture::setTexture(SDL_Texture *t) {
    _texture = t;
}
void Texture::setSDLRenderer(SDL_Renderer *sdlr) {
    _sdl_renderer = sdlr;
}