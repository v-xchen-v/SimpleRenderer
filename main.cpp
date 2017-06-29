#include "../SDL/include/SDL.h"     //添加SDL头文件
#include "stdio.h"
#undef main                         //SDL中的SDL_main.h已经定义了main

const int baseWindow_w = 800;
const int baseWindow_h = 600;
SDL_Renderer *gRenderer;
SDL_Window *baseWindow;
int init()
{
    //the window we'll be rendering to
    baseWindow = NULL;

    baseWindow = SDL_CreateWindow("Simple Renderer @xchen",//窗口标题
                                  SDL_WINDOWPOS_UNDEFINED,//窗口位置
                                  SDL_WINDOWPOS_UNDEFINED,
                                  baseWindow_w,baseWindow_h,//窗口的宽、高
                                  SDL_WINDOW_SHOWN);
    if(baseWindow == NULL)
    {
        printf("baseWindow could not be created! SDL Error:%s\n",SDL_GetError());
        return false;
    }

    //A structure that contains a rendering state.
    gRenderer = SDL_CreateRenderer(baseWindow,-1,SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        printf("gRenderer could not be created! SDL Error:$s\n",SDL_GetError());
        return false;
    }
}
void loop()
{
    bool quit = false;
    SDL_Event e;
    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xff);
        //Clear the entire screen to our selected color
        SDL_RenderClear(gRenderer);

        //在随机位置上画随机颜色的1000个点
        for(int i=0;i<1000;i++)
        {
            int r = rand()/100 % 256;
            int g = rand() / 100 % 256;
            int b = rand() / 100 % 256;
            int col = rand() % baseWindow_w;
            int row = rand() % baseWindow_h;
            // 设置颜色
            SDL_SetRenderDrawColor(gRenderer, (Uint8)r, (Uint8)g, (Uint8)b, 0xff);
            // 画点
            SDL_RenderDrawPoint(gRenderer, col,row);
        }
        //Use this function to update the screen with any rendering performed since the previous cal
        SDL_RenderPresent(gRenderer);
    }
}
void close()
{
    //destroy the rendering context for a window and free associated textures
    SDL_DestroyRenderer(gRenderer);
    //destroy a window.
    SDL_DestroyWindow(baseWindow);
    //clean up all initialized subsystems. You should call it upon all exit conditions
    SDL_Quit();
}

int main()
{
    init();
    loop();
    close();
}
