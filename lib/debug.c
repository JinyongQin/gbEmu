#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

extern rQueue serial_buffer;

/**
 * 初始化调试窗口
 * @param win 调试窗口结构体指针
 * @return 成功返回0，失败返回-1
 */
int init_debugWindow(debugWin *win)
{
    // 参数检查
    if (!win)
        return -1;

    // 设置默认值
    if (win->width <= 0)
        win->width = 640;
    if (win->height <= 0)
        win->height = 480;
    if (win->x < 0)
        win->x = SDL_WINDOWPOS_CENTERED;
    if (win->y < 0)
        win->y = SDL_WINDOWPOS_CENTERED;

    // 初始化颜色 (白色文字，黑色背景)
    win->text_color = (SDL_Color){255, 255, 255, 255};
    win->bg_color = (SDL_Color){0, 0, 0, 255};

    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL初始化失败: %s\n", SDL_GetError());
        return -1;
    }

    // 初始化SDL_ttf
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "SDL_ttf初始化失败: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // 创建窗口
    win->window = SDL_CreateWindow(
        "debug window",
        win->x,
        win->y,
        win->width,
        win->height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!win->window)
    {
        fprintf(stderr, "窗口创建失败: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    // 创建渲染器
    win->renderer = SDL_CreateRenderer(win->window, -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!win->renderer)
    {
        fprintf(stderr, "渲染器创建失败: %s\n", SDL_GetError());
        SDL_DestroyWindow(win->window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    win->font = TTF_OpenFont("/home/yong/gameboy/NotoSansMono-Medium.ttf", 24);
    // 初始化文本相关
    win->text = NULL;
    win->text_texture = NULL;
    return 0;
}

/**
 * 设置调试窗口文本
 * @param win 调试窗口结构体指针
 * @param text 要显示的文本
 */
void set_debugText(debugWin *win, const char *text)
{
    if (!win || !text)
        return;
    // 释放旧文本内存
    if (win->text)
    {
        free(win->text);
        win->text = NULL;
    }
    // 释放旧纹理
    if (win->text_texture)
    {
        SDL_DestroyTexture(win->text_texture);
        win->text_texture = NULL;
    }
    // 复制新文本
    win->text = strdup(text);
    if (!win->text)
        return;
    // 创建新纹理
    SDL_Surface *surface = TTF_RenderText_Blended(win->font, win->text, win->text_color);
    if (surface)
    {
        win->text_texture = SDL_CreateTextureFromSurface(win->renderer, surface);
        SDL_FreeSurface(surface);
    }
}

// set serial port data to text
static int win_text_idx = 0; // 记录当前文本索引

void blend_serialText(debugWin *win)
{
    while (rQueue_is_empty(&serial_buffer) == false)
    {
        win->text[win_text_idx] = rQueue_pop(&serial_buffer);
        win_text_idx++;
        if (win_text_idx >= 255)
        {
            win_text_idx = 0;
            break;
        }
    }
    // blend win->text
    win->text[win_text_idx] = '\0';
    // 释放旧纹理
    if (win->text_texture)
    {
        SDL_DestroyTexture(win->text_texture);
        win->text_texture = NULL;
    }
    // 创建新纹理
    SDL_Surface *surface = TTF_RenderText_Blended(win->font, win->text, win->text_color);
    if (surface)
    {
        win->text_texture = SDL_CreateTextureFromSurface(win->renderer, surface);
        SDL_FreeSurface(surface);
    }
}

/**
 * 更新调试窗口
 * @param win 调试窗口结构体指针
 */
void update_debugWindow(debugWin *win)
{
    // get serial port data
    if (!win || !win->renderer)
        return;
    // 获取当前窗口尺寸
    // SDL_GetWindowSize(win->window, &win->width, &win->height);
    // 清屏
    SDL_SetRenderDrawColor(win->renderer,
                           win->bg_color.r,
                           win->bg_color.g,
                           win->bg_color.b,
                           win->bg_color.a);
    SDL_RenderClear(win->renderer);
    // 渲染文本
    if (win->text_texture)
    {

        int texW, texH;
        SDL_QueryTexture(win->text_texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = {
            0,
            0,
            texW,
            texH};
        SDL_RenderCopy(win->renderer, win->text_texture, NULL, &dstrect);
    }
    // 更新显示
    SDL_RenderPresent(win->renderer);
}

/**
 * 销毁调试窗口并释放资源
 * @param win 调试窗口结构体指针
 */
void destroy_debugWindow(debugWin *win)
{
    if (!win)
        return;
    if (win->text)
    {
        free(win->text);
        win->text = NULL;
    }
    if (win->text_texture)
    {
        SDL_DestroyTexture(win->text_texture);
        win->text_texture = NULL;
    }
    if (win->font)
    {
        TTF_CloseFont(win->font);
        win->font = NULL;
    }
    if (win->renderer)
    {
        SDL_DestroyRenderer(win->renderer);
        win->renderer = NULL;
    }
    if (win->window)
    {
        SDL_DestroyWindow(win->window);
        win->window = NULL;
    }
    TTF_Quit();
    SDL_Quit();
}

void dbg_update()
{
}

void dbg_print()
{
}