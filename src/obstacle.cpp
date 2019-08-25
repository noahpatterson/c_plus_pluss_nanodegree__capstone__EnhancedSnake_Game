#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include "obstacle.h"
#include "texture.h"
#include "Helpers.h"
// #include <iostream>

Obstacle::Obstacle(SDL_Renderer *sdl_renderer, Clip c)
    : Texture(sdl_renderer),
      _clipType(c) {}

bool Obstacle::loadFromFile(std::string path)
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( getSDLRenderer(), loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            setWidth(loadedSurface->w);
            setHeight(loadedSurface->h);
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    setTexture(newTexture);
    return getTexture() != NULL;
}

void Obstacle::render(int x, int y)
{
  //render sprites
  if(!loadMedia())
  {
    printf( "Failed to load media!\n" );
  }
  else
  {

       //Render top right sprite
       // spriteSheetTexture.render(screen_width - spriteClips[ 1 ].w, 0);

    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, getWidth(), getHeight() };

    //Set clip rendering dimensions
    // if(_spriteClip)
    // {
        renderQuad.w = _spriteClip.w;
        renderQuad.h = _spriteClip.h;
    // }

    //Render to screen
     SDL_RenderCopy(getSDLRenderer(), getTexture(), &_spriteClip, &renderQuad );
  }
}

bool Obstacle::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !loadFromFile( "../images/snake_game_sprite.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
    switch (_clipType) {
      case Clip::donut: {
        //Set donut sprite
        _spriteClip.x = 0;
        _spriteClip.y = 0;
        _spriteClip.w = 40;
        _spriteClip.h = 40;
        break;
      }
      case Clip::bomb: {
        //Set bomb sprite
        _spriteClip.x = 0;
        _spriteClip.y = 40;
        _spriteClip.w = 36;
        _spriteClip.h = 37;
        break;
      }
      default: break;
    }
	}

	return success;
}

bool Obstacle::checkCollision(RandomPoint &point, int &screen_width, int &screen_height, int &grid_width, int &grid_height, int &new_x, int &new_y) {
  int pointX =  point.x / (screen_width / grid_width);
  int pointY =  point.y / (screen_height / grid_height);
  if ((pointX == new_x && pointY == new_y) ||
      (pointX + 1 == new_x && pointY +1 == new_y) ||
      (pointX - 1 == new_x && pointY - 1 == new_y)) {
        return true;
  }
  return false;
}