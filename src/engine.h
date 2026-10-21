#pragma once
#include <SDL.h>
#include <vector>

using namespace std;

//window config
const int width = 500;
const int height = 500;

//making RGB
inline Uint32 RGB(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255){
    return ((Uint32)a << 24) | ((Uint32)b << 16) | ((Uint32)g << 8) | (Uint32)r;
}

struct LineData{
    int x1; // 1 - start point
    int x2; // 2 - end point
    int y1;
    int y2;
    uint8_t r;   // uint8_t - 8 bit
    uint8_t g;
    uint8_t b;
};

/*
inline void Drawverticalline(vector<Uint32> & framebuffer, int x, int y0, int y1, Uint32 color){
    if (y0 > y1){
        swap(y0, y1);
    }
    if (x < 0||x >= width){
        return;
    }
    for(int i = y0; i <= y1; i++ ){
        if(i >= 0 && i < height) {
            framebuffer[i * width + x] = color;
        }
    }
}
*/

//make it all black ("clean")
inline void clean(vector<Uint32>& framebuffer, Uint32 color){
    fill(framebuffer.begin(), framebuffer.end(), color);
}
inline void Drawpixel(vector<Uint32>& framebuffer, int x, int y, Uint32 color){
    if (x >= 0 && x < width && y >= 0 && y < height){
        framebuffer[y * width + x] = color;
    }
}
inline void Drawline(vector<Uint32>& framebuffer, int x0, int y0, int x1, int y1, Uint32 color){
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    //Search for the biggest line
    //abs-ignores the negative value
    int abs_dx = abs(dx);
    int abs_dy = abs(dy);
    int steps;

    if (abs_dx > abs_dy) {
        steps = abs_dx;
    } else {
        steps = abs_dy;
    }
    if (steps == 0) {
        Drawpixel(framebuffer, x0, y0, color);
        return;
    }

    //looking for one step
    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    
    float x = x0;
    float y = y0;

    //go step by step and color the pixels
    for (int i = 0; i <= steps; i++) {
        //                     C-STYLE CAST (int)(x + 0.5f)     for ex. 11.9 + 0.5 = 12.4 = 12 (ROUNDING)
        Drawpixel(framebuffer, (int)(x + 0.5f), (int)(y + 0.5f), color); // +0.5 needs for rounding
        x += xInc;
        y += yInc;
    }
}

