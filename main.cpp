#include "../SDL/include/SDL.h"     //添加SDL头文件
#include "stdio.h"
#include "math.h"
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
void drawPoint(SDL_Renderer *gRenderer,int col,int row,Uint8 R,float G,Uint8 B,Uint8 A)
{
    // 设置颜色
    SDL_SetRenderDrawColor(gRenderer, R, G, B, A);
    // 画点
    SDL_RenderDrawPoint(gRenderer, col,row);
}
void drawLine_DDA(SDL_Renderer *gRenderer,float x1,float y1,float x2,float y2,Uint8 R,float G,Uint8 B,Uint8 A)
{
    float dm = 0,dx = 0,dy = 0;
    if(fabs(x2-x1) >= fabs(y2-y1))
    {
        dm = fabs(x2-x1);
    }else
    {
        dm = fabs(y2-y1);
    }
    dx = (float)(x2-x1)/(dm);
    dy = (float)(y2-y1)/(dm);
    for(float i=0;i<dm;i++)
    {
        drawPoint(gRenderer,x1,y1,R,G,B,A);
        x1+=dx;
        y1+=dy;
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

        drawLine_DDA(gRenderer,0,0,baseWindow_w,baseWindow_h,255,255,0,0xff);

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
