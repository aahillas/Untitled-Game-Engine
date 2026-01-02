// 2D Graphics Renderer (Prototypes)

#ifndef RNDR2D_H
#define RNDR2D_H

// ****************************************************************************
// * Includes & Namespaces
// ****************************************************************************

#include <SDL3/SDL.h>

// ****************************************************************************
// * Classes
// ****************************************************************************

class Rndr2D
{
};

class Texture
{
    private:
        SDL_Surface* imgOriginal;               // Original Image
        SDL_Surface* imgConvert;                // Converted Image (ARGB8888)
        Uint32*      pxlArray1D;                // 1-Dimensional Pixel Array (Converted Image)
        Uint32**     pxlArray2D;                // 2-Dimensional Pixel Array (Converted Image)
        int          width;                     // Image Width
        int          height;                    // Image Height

    public:
         Texture(const char*, int, int);        // Constructor Parameters - Image File Name, Width, Height
        ~Texture();

        Uint32       returnPixel(int, int);     // Returns Pixel from Coordinate (Column (Y), Row (X))
};

#endif // RNDR2D_H
