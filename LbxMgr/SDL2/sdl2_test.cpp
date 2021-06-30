// #include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <cstdio>

#include "SDL.h"

#include "sdl2_test.h"

// Screen dimension constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 200;

void sdl2_test()
{
    printf("SDL2 Version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
    // printf("SDL2_IMAGE Version: %d.%d.%d", SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL);

    // The window we'll be rendering to
    SDL_Window* window = NULL;

    // The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        // Create window
        window = SDL_CreateWindow( "SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            // Get window surface
            screenSurface = SDL_GetWindowSurface( window );

            // Fill the surface white
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

            // Update the surface
            SDL_UpdateWindowSurface( window );

            // Wait two seconds
            SDL_Delay( 2000 );
        }
    }

    // Destroy window
    SDL_DestroyWindow( window );

    // Quit SDL subsystems
    SDL_Quit();
}
