//#include <stdio.h>          /* fclose(), fopen(), fread(), fwrite(), printf(); FILE */
#include <iostream>

#include <SDL.h>

#include "sdl_colormap.h"
#include "mom_main_colormap.h"

SDL_Color colormap_window_background_color = {8, 4, 4, SDL_ALPHA_OPAQUE};    // light black
SDL_Color colormap_cell_border_color = {44, 44, 44, SDL_ALPHA_OPAQUE};      // dark grey

#define GRID_UNIT_SIZE_WIDTH 8      // x; 320 / 8 = 40
#define GRID_UNIT_SIZE_HEIGHT 5     // y; 200 / 5 = 40

struct s_GUI_PARAM {
    int colormap_window_width;
    int colormap_window_height;
    int horizontal_cell_count;
    int vertical_cell_count;
    int cell_count;
    int cell_border_width;
    int cell_width;
    int cell_height;
    int window_width;
    int window_height;
};
typedef struct s_GUI_PARAM GUI_PARAM;

int sdl_colormap()
{
    GUI_PARAM gui_param;
    gui_param.colormap_window_width = 640;
    gui_param.colormap_window_height = 400;
    gui_param.horizontal_cell_count = 16;
    gui_param.vertical_cell_count = 16;
    gui_param.cell_count = gui_param.horizontal_cell_count * gui_param.vertical_cell_count;
    gui_param.cell_border_width = 2;
    gui_param.cell_width = ((gui_param.colormap_window_width - gui_param.cell_border_width) - (gui_param.horizontal_cell_count * gui_param.cell_border_width)) / gui_param.horizontal_cell_count;
    gui_param.cell_height = ((gui_param.colormap_window_height - gui_param.cell_border_width) - (gui_param.vertical_cell_count * gui_param.cell_border_width)) / gui_param.vertical_cell_count;
    gui_param.window_width = (gui_param.cell_border_width * gui_param.horizontal_cell_count) + (gui_param.cell_width * gui_param.horizontal_cell_count) + gui_param.cell_border_width;
    gui_param.window_height = (gui_param.cell_border_width * gui_param.vertical_cell_count) + (gui_param.cell_height * gui_param.vertical_cell_count) + gui_param.cell_border_width;

/*
    printf("gui_param.cell_width: %d", gui_param.cell_width);
    printf("gui_param.cell_height: %d", gui_param.cell_height);
    printf("gui_param.window_width: %d", gui_param.window_width);
    printf("gui_param.window_height: %d", gui_param.window_height);
*/

    std::cout << "gui_param.cell_width: " << gui_param.cell_width << std::endl;
    std::cout << "gui_param.cell_height: " << gui_param.cell_height << std::endl;
    std::cout << "gui_param.window_width: " << gui_param.window_width << std::endl;
    std::cout << "gui_param.window_height: " << gui_param.window_height << std::endl;

    int cell_index_current;

    int itr_colormap_x_count;
    int itr_colormap_y_count;
    int itr_colormap_count;

    SDL_Rect colormap_array[gui_param.cell_count];

    for (itr_colormap_y_count = 0; itr_colormap_y_count < gui_param.vertical_cell_count; itr_colormap_y_count++)
    {
        for (itr_colormap_x_count = 0; itr_colormap_x_count < gui_param.horizontal_cell_count; itr_colormap_x_count++) {
            //itr_colormap_count = (itr_colormap_x_count + (itr_colormap_x_count * itr_colormap_y_count)) + ((16 - itr_colormap_x_count) * itr_colormap_y_count);
            itr_colormap_count = (itr_colormap_x_count + (itr_colormap_x_count * itr_colormap_y_count)) + ((gui_param.horizontal_cell_count - itr_colormap_x_count) * itr_colormap_y_count);

            colormap_array[itr_colormap_count].x = gui_param.cell_width * itr_colormap_x_count;
            colormap_array[itr_colormap_count].y = gui_param.cell_height * itr_colormap_y_count;
            colormap_array[itr_colormap_count].h = gui_param.cell_height;
            colormap_array[itr_colormap_count].w = gui_param.cell_width;
        }
    }

    // SDL_Init(SDL_INIT_VIDEO);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    //const char *colormap_window_title;
    const char *colormap_window_title = "SDL ColorMap";
    int colormap_window_x = SDL_WINDOWPOS_CENTERED;
    int colormap_window_y = SDL_WINDOWPOS_CENTERED;
    int colormap_window_w = gui_param.window_width;
    int colormap_window_h = gui_param.window_height;
    Uint32 colormap_window_flags = 0;

    SDL_Window *colormap_window = SDL_CreateWindow(colormap_window_title,
                                                   colormap_window_x,
                                                   colormap_window_y,
                                                   colormap_window_w,
                                                   colormap_window_h,
                                                   colormap_window_flags);

    //SDL_Window *colormap_renderer_window;
    int colormap_renderer_index = -1; // -1 to initialize the first one supporting the requested flags
    Uint32 colormap_renderer_flags = SDL_RENDERER_SOFTWARE;
    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Renderer *colormap_renderer = SDL_CreateRenderer(colormap_window,
                                                         colormap_renderer_index,
                                                         colormap_renderer_flags);

    // SDL_Surface* SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
    // SDL_Surface* SDL_CreateRGBSurfaceWithFormat(Uint32 flags, int width, int height, int depth, Uint32 format);
    // SDL_Surface* SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
    Uint32 colormap_surface_flags = 0;
    int colormap_surface_width = gui_param.colormap_window_width;
    int colormap_surface_height = gui_param.colormap_window_height;
    int colormap_surface_depth = 8;
    Uint32 colormap_surface_Rmask = 0;
    Uint32 colormap_surface_Gmask = 0;
    Uint32 colormap_surface_Bmask = 0;
    Uint32 colormap_surface_Amask = 0;
    SDL_Surface *colormap_surface = SDL_CreateRGBSurface(colormap_surface_flags,
                                                         colormap_surface_width,
                                                         colormap_surface_height,
                                                         colormap_surface_depth,
                                                         colormap_surface_Rmask,
                                                         colormap_surface_Gmask,
                                                         colormap_surface_Bmask,
                                                         colormap_surface_Amask);
    /*
    WTF, mate?
    SDL_Surface* surface -> SDL_PixelFormat* format -> SDL_Palette* palette -> SDL_Color* colors
    */
    SDL_SetPaletteColors(colormap_surface->format->palette, mom_main_colormap_sdl_color, 0, 256);

    //    //SDL_Surface* SDL_LoadBMP(const char* file);
    //    SDL_Surface* bmp_surface = SDL_LoadBMP("button-gold-up-save_bmp.bmp");
    //    if (bmp_surface == NULL) {
    //        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_LoadBMP: %s", SDL_GetError());
    //        return EXIT_FAILURE;
    //    }
    //    SDL_Surface* colormap_window_surface = SDL_GetWindowSurface(colormap_window);
    //    SDL_BlitSurface(bmp_surface, NULL, colormap_window_surface, NULL);
    //    SDL_UpdateWindowSurface(colormap_window);
    //    SDL_Delay(1200);
    //
    //    //    SDL_Rect* controls_panel;
    //    //    SDL_Rect* information_panel;
    //    //    SDL_Rect* colormap_panel;
    //    //    controls_panel->x = 10;
    //    //    controls_panel->y = 10;
    //    //    controls_panel->w = 300;
    //    //    controls_panel->h = 40;
    //    //    information_panel->x = 270;
    //    //    information_panel->y = 60;
    //    //    information_panel->w = 40;
    //    //    information_panel->h = 130;
    //    //    colormap_panel->x = 10;
    //    //    colormap_panel->y = 60;
    //    //    colormap_panel->w = 140;
    //    //    colormap_panel->h = 130;
    //    //
    //    //    //    // https://wiki.libsdl.org/SDL_RenderCopy
    //    //    //    // int SDL_RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect);
    //    //    //    SDL_Rect rect;
    //    //    //    rect.x=rect.y=0;
    //    //    //    rect.w=360;
    //    //    //    rect.h=240;
    //    //    //    SDL_RenderFillRect(ren, &rect);
    //    //    //    star.setDest(100, 100, 100, 120);
    //    //    //    star.setSource(0, 0, 75, 50);
    //    //    //    star.setImage("image.png", ren);
    //    //    //    SDL_Rect dest = o.getDest();
    //    //    //    SDL_Rect src = o.getSource();
    //    //    //    SDL_RenderCopyEx(ren, o.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);


    SDL_Event event;

    SDL_SetRenderDrawColor(colormap_renderer, colormap_window_background_color.r, colormap_window_background_color.g, colormap_window_background_color.b, colormap_window_background_color.a);
    SDL_RenderClear(colormap_renderer);
    SDL_RenderPresent(colormap_renderer);

    SDL_SetRenderDrawColor(colormap_renderer, colormap_cell_border_color.r, colormap_cell_border_color.g, colormap_cell_border_color.b, colormap_cell_border_color.a);
    SDL_RenderDrawRects(colormap_renderer, colormap_array, gui_param.cell_count);
    SDL_RenderPresent(colormap_renderer);
    SDL_Delay(1200);

    while(true)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        SDL_SetRenderDrawColor(colormap_renderer, colormap_window_background_color.r, colormap_window_background_color.g, colormap_window_background_color.b, colormap_window_background_color.a);
        SDL_RenderClear(colormap_renderer);
        for (itr_colormap_count = 0; itr_colormap_count < gui_param.cell_count; itr_colormap_count++)
        {
            SDL_SetRenderDrawColor(colormap_renderer, mom_main_colormap_sdl_color[itr_colormap_count].r, mom_main_colormap_sdl_color[itr_colormap_count].g, mom_main_colormap_sdl_color[itr_colormap_count].b, mom_main_colormap_sdl_color[itr_colormap_count].a);
            SDL_RenderFillRect(colormap_renderer, &colormap_array[itr_colormap_count]);
        }
        SDL_RenderPresent(colormap_renderer);

    }

    if(SDL_SaveBMP(colormap_surface, "colormap_surface.bmp") != 0)
    {
        // Error saving bitmap
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }

    SDL_DestroyRenderer(colormap_renderer);
    SDL_DestroyWindow(colormap_window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
