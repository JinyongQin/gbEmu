#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
typedef struct debugWin
{
    int width;
    int height;
    int x;
    int y;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *text_texture;
    char *text;
    SDL_Color text_color;
    SDL_Color bg_color;
} debugWin;
int init_debugWindow(debugWin *win);
void set_debugText(debugWin *win, const char *text);
void update_debugWindow(debugWin *win);
void destroy_debugWindow(debugWin *win);

void blend_serialText(debugWin *win);