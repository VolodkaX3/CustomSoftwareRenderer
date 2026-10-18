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
    int x1;
    int x2;
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
        std::cerr << "Error with open file!!!\n";
        return;
    }

    vector<LineData> lines = {
        {100, 100, 700, 100, 255, 0, 0},
        {700, 100, 700, 500, 0, 255, 0},
        {700, 500, 100, 500, 0, 0, 255},
        {100, 500, 100, 100, 255, 255, 0},
    };

    for(const auto& line : lines ){
        outFile.write(reinterpret_cast<const char*>(&line),sizeof(LineData));
    }
    outFile.close();
    
}