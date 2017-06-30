#include "../SDL/include/SDL.h"     //添加SDL头文件
#include "stdio.h"
#include "math.h"
#include "iostream"
#include "vector"
#include "algorithm"
#undef main                         //SDL中的SDL_main.h已经定义了main

using namespace std;
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
void drawTriangle_Line(SDL_Renderer *gRenderer,int point1_x, int point1_y, int point2_x,int point2_y,int point3_x,int point3_y,Uint8 R,Uint8 G,Uint8 B,Uint8 A)
{
    //绘制线框三角形
    drawLine_DDA(gRenderer,point1_x,point1_y,point2_x,point2_y,R,G,B,A);
    drawLine_DDA(gRenderer,point2_x,point2_y,point3_x,point3_y,R,G,B,A);
    drawLine_DDA(gRenderer,point3_x,point3_y,point1_x,point1_y,R,G,B,A);
}
//SDL的坐标系是坐上角为原点
/*
         (x1,y1)
        /      \
       /        \
      /          \
  (x2,y2)——————-(x3,y3)
  y不断增加，增至yi的时候，Xl = (yi-y1)*(x2-x1)/(y2-y1);
                        Xr = (yi-y1)*(x3-x1)/(y3-y1);

*/
void drawTriangle_flatBottom(SDL_Renderer *gRenderer,float point1_x, float point1_y, float point2_x,float point2_y,float point3_x,float point3_y,Uint8 R,Uint8 G,Uint8 B,Uint8 A)
{
    for(float i = point1_y; i<=point2_y ;i++)
    {
        float xl = (i-point1_y)*(point2_x-point1_x)/(point2_y-point1_y)+point1_x;
        float xr = (i-point1_y)*(point3_x-point1_x)/(point3_y-point1_y)+point1_x;
        cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatBottom"<<endl;
        drawLine_DDA(gRenderer,xl,i,xr,i,R,G,B,A);
    }
}

/*
        (x1,y1)--------(x2,y2)
            \             /
             \           /
              \         /
               \       /
                (x3,y3)
*/
void drawTriangle_flatTop(SDL_Renderer *gRenderer,float point1_x, float point1_y, float point2_x,float point2_y,float point3_x,float point3_y,Uint8 R,Uint8 G,Uint8 B,Uint8 A)
{
    for(float i = point1_y; i<=point3_y ;i++)
    {
        float xl = (i-point3_y)*(point1_x-point3_x)/(point1_y-point3_y)+point3_x;
        float xr = (i-point3_y)*(point2_x-point3_x)/(point2_y-point3_y)+point3_x;
        // cout<<xl<<" "<<xr<<" "<<i<<"xr drawTriangle_flatTop"<<endl;
        drawLine_DDA(gRenderer,xl,i,xr,i,R,G,B,A);
    }
}
/*
                (xtop,ytop)
                      /|
                     / |
                    /  |
                   /   |
                  /____|
      (xmid,ymid) \    |(xtmp,ymid)
                   \   |
                    \  |
                     \ |
                      \|
                      (xbottom,ybottom)
*/
bool comp(pair<float,float>y_pair1,pair<float,float>y_pair2)
{
    return y_pair1.second<y_pair2.second; //float浮点数比较？
}
void drawTriangle(SDL_Renderer *gRenderer,float point1_x, float point1_y, float point2_x, float point2_y, float point3_x, float point3_y,Uint8 R,Uint8 G,Uint8 B,Uint8 A)
{
    float xtop,ytop,xmid,ymid,xtmp,xbottom,ybottom;
    if(point1_y == point2_y)
    {
        if(point3_y<=point1_y)
        {
            drawTriangle_flatBottom(gRenderer,point3_x,point3_y,point1_x,point1_y,point2_x,point2_y,R,G,B,A);
        }else
        {
            drawTriangle_flatTop(gRenderer,point1_x,point1_y,point2_x,point2_y,point3_x,point3_y,R,G,B,A);
        }
    }else if(point1_y == point3_y)
    {
        if(point2_y <= point1_y)
        {
            drawTriangle_flatBottom(gRenderer,point2_x,point2_y,point1_x,point1_y,point3_x,point3_y,R,G,B,A);
        }else
        {
            drawTriangle_flatTop(gRenderer,point1_x,point1_y,point3_x,point3_y,point2_x,point2_y,R,G,B,A);
        }
    }else if(point2_y == point3_y)
    {
        if(point1_y<=point2_y)
        {
            drawTriangle_flatBottom(gRenderer,point1_x,point1_y,point2_x,point2_y,point3_x,point3_y,R,G,B,A);
        }else
        {
            drawTriangle_flatTop(gRenderer,point2_x,point2_y,point3_x,point3_y,point1_x,point1_y,R,G,B,A);
        }
    }else //如果不是平顶、平底三角形
    {
        vector<pair<float,float>>vc;
        vc.push_back(make_pair(point1_x,point1_y));
        vc.push_back(make_pair(point2_x,point2_y));
        vc.push_back(make_pair(point3_x,point3_y));
        sort(vc.begin(),vc.end(),comp);
        xtop = vc[0].first;
        ytop = vc[0].second;
        xmid = vc[1].first;
        ymid = vc[1].second;
        xbottom = vc[2].first;
        ybottom = vc[2].second;
        xtmp = (ymid-ytop)*(xbottom-xtop)/(ybottom-ytop)+xtop;
        // cout<<xtop<<" "<<ytop<<" "<<xmid<<" "<<ymid<<" "<<xbottom<<" "<<ybottom<<" "<<xtmp<<endl;
        drawTriangle_flatBottom(gRenderer,xtop,ytop,xtmp,ymid,xmid,ymid,R,G,B,A);
        drawTriangle_flatTop(gRenderer,xmid,ymid,xtmp,ymid,xbottom,ybottom,R,G,B,A);
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

        drawTriangle(gRenderer,200,200,100,300,300,300,255,0,0,0xff);

        drawTriangle(gRenderer,400,400,600,400,500,500,255,255,0,0xff);

        drawTriangle(gRenderer,400,200,300,250,700,300,0,0,0xff,0xff);

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
