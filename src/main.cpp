#include <SDL.h>
#include <vector>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

//making RGB
inline Uint32 RGB(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255){
    return ((Uint32)a << 24) | ((Uint32)b << 16) | ((Uint32)g << 8) | (Uint32)r;
}

void Draw_vertical_line(std::vector<Uint32> & framebuffer, int x, int y0, int y1, Uint32 color){
    //antifooldefence
    if (y0 > y1){
        std::swap(y0, y1);
    }

    if (x < 0 || x >= WIDTH) {
        return;
    }

    for (int y = y0; y <= y1; y++) {
        if (y >= 0 && y < HEIGHT) {
            framebuffer[y * WIDTH + x] = color;
        }
    }
}

//Delete func
void ClearScreen(std::vector<Uint32>& framebuffer, Uint32 color) {
    std::fill(framebuffer.begin(), framebuffer.end(), color);
}

// Drawing one pixel func with checking screen borders
void DrawPixel(std::vector<Uint32>& framebuffer, int x, int y, Uint32 color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        framebuffer[y * WIDTH + x] = color;
    }
}

// draw line bresenham
void DrawLine(std::vector<Uint32>& framebuffer, int x0, int y0, int x1, int y1, Uint32 color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        DrawPixel(framebuffer, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "C++ Custom Software Renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT
    );

    std::vector<Uint32> framebuffer(WIDTH * HEIGHT, 0xFF000000); // black screen

    bool running = true;
    SDL_Event event;
/*
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // White dot
        framebuffer[HEIGHT / 2 * WIDTH + WIDTH / 2] = 0xFFFFFFFF; 

        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), WIDTH * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }*/

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        //-----------------------------
        // here we can draw something:
        //----------------------------

        // clear old frame
        ClearScreen(framebuffer, 0xFF000000);

        // draw line across screen
        DrawLine(framebuffer, 100, 100, 700, 500, RGB(255, 85, 85));

        DrawLine(framebuffer, 0, 300, 800, 300, RGB(0, 115, 255)); //rgb(0, 115, 255);

        // line from a specific function
        Draw_vertical_line(framebuffer, 200, 50, 450, RGB(0, 255, 0));

        // render buffer to window
        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), WIDTH * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}