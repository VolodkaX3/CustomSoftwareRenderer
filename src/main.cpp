#include <SDL.h>
#include <vector>
#include <iostream>
#include <fstream>

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

void Binfilefunc(const string& filename){
    //open file with content delete
    ofstream outFile(filename, ios::out | ios::binary | ios::trunc);

    //checking
    if (!outFile.is_open()) {
        cerr << "Error with open file!!!\n";
        return;
    }

    vector<LineData> lines = {  // {x1,y1,x2,y2,r,g,b},
        /* //line
        {100, 100, 400, 400, 255, 100, 100},
        {400, 100, 100, 400, 100, 255, 100},
        {100, 250, 400, 250, 100, 100, 255},
        {250, 100, 250, 400, 255, 255, 0},
        */
        //figure
        {150, 150, 350, 150, 255, 50, 50},
        {350, 150, 350, 350, 50, 255, 50},
        {350, 350, 150, 350, 50, 50, 255},
        {150, 350, 150, 150, 255, 255, 0},

        {250, 150, 350, 250, 0, 255, 255},
        {350, 250, 250, 350, 255, 0, 255},
        {250, 350, 150, 250, 255, 128, 0},
        {150, 250, 250, 150, 128, 0, 255},

        {150, 150, 350, 350, 255, 255, 255},
        {350, 150, 150, 350, 255, 255, 255},
    };

    for(const auto& line : lines ){
        //Trick the compiler by making it think the data structure is a regular byte array
        outFile.write(reinterpret_cast<const char*>(&line),sizeof(LineData));
    }
    outFile.close();
    }

void Drawverticalline(vector<Uint32> & framebuffer, int x, int y0, int y1, Uint32 color){
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
//make it all black ("clean")
void clean(vector<Uint32>& framebuffer, Uint32 color){
    fill(framebuffer.begin(), framebuffer.end(), color);
}
void Drawpixel(vector<Uint32>& framebuffer, int x, int y, Uint32 color){
    if (x >= 0 && x < width && y >= 0 && y < height){
        framebuffer[y * width + x] = color;
    }
}
void Drawline(vector<Uint32>& framebuffer, int x0, int y0, int x1, int y1, Uint32 color){
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
#undef main
int main(){
    string filename = "lines_db.bin";
    Binfilefunc(filename);
    //start SDL video system
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error:" << SDL_GetError() << "\n";
        return 1;
    }
    //create main SDL window 
    SDL_Window* window = SDL_CreateWindow(
        "C++ Custom Software Renderer | by Kostiak",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN
    );
    //if the window didnt open 
    if (!window) {
        std::cerr << "Window couldn't be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }
    //renderer — an SDL tool that renders our texture in the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    /*texture — a special SDL texture.
    We send the contents of our framebuffer to it so that the graphics card knows which colors to display*/
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height
    );


    vector<Uint32> framebuffer(width * height, 0xFF000000);

    //variables for contol
    bool running = true;
    //special structure from SDL library, which can track what user do
    SDL_Event event;

    FILE* file = fopen("lines_db.bin", "rb");
    if (file != nullptr) {
        LineData line;
        int index = 0;
    
        // 1-because we have variable NOT MASS
        while (fread(&line, sizeof(LineData), 1, file) == 1) {
        cout << "line " << index++ << ": " << "x1=" << line.x1 << ", y1=" << line.y1 << " | " << "x2=" << line.x2 << ", y2=" << line.y2 << " | " << "rgb(" << (int)line.r << ", " << (int)line.g << ", " << (int)line.b << ")\n";
        }
        fclose(file);
    } else {
        cout << "Error with file open!\n";
    }

    while (running) {
        //processing window events  for ex. close
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clean(framebuffer, 0xFF000000);

        //open bin file for read
        ifstream inFile(filename, ios::in | ios::binary);
        
        FILE* file = fopen(filename.c_str(), "rb");
        if (file != nullptr) {
            LineData line;
            while (fread(&line, sizeof(LineData), 1, file) == 1) {
                Drawline(framebuffer, line.x1, line.y1, line.x2, line.y2, RGB(line.r, line.g, line.b));
            }
            fclose(file);
        } else {
            std::cerr << "Error with open file for reading! \n";
        }

        //texture update
        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(Uint32));
        //renderer clean
        SDL_RenderClear(renderer);
        //copy textur for window
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        // SHOW ALL ON SCREEN
        SDL_RenderPresent(renderer);
    }

    //correct memory freeing and exiting SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}