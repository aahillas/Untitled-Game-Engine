// 2D Graphics Renderer (Methods)

// ****************************************************************************
// * Includes & Namespaces
// ****************************************************************************

#include "rndr2D.h"

#include <cstdio>
#include <SDL3_image/SDL_image.h>

// ****************************************************************************
// * Class Methods (Texture)
// ****************************************************************************

Texture::Texture(const char* fileName, int imgWidth, int imgHeight)
{
// Image Loading & Error Handling
    imgOriginal = IMG_Load(fileName);

    if (!imgOriginal)
    {
        printf("IMG_Load failed: %s\n", SDL_GetError());
        exit(1);
    }

// Convert Image to ARGB8888 & Error Handling
    imgConvert = SDL_ConvertSurface(imgOriginal, SDL_PIXELFORMAT_ARGB8888);

    if (!imgConvert)
    {
        printf("Convert failed: %s\n", SDL_GetError());
        exit(1);
    }

// Set Image Width & Height    
    width       = imgWidth;
    height      = imgHeight;

// Point to SDL_ConvertSurface Pixel Array
    pxlArray1D = (Uint32*)imgConvert->pixels;

// Allocate 2-Dimensional Pixel Array
    pxlArray2D = new Uint32*[height];

    for (int y = 0; y < height; y++)
        pxlArray2D[y] = new Uint32[width];

// Define 2-Dimensional Pixel Array from 1-Dimensional Pixel Array
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            pxlArray2D[y][x] = pxlArray1D[y * width + x];
}

Texture::~Texture()
{
    for (int y = 0; y < height; y++)
        delete[] pxlArray2D[y];

    delete[] pxlArray2D;

    SDL_DestroySurface(imgConvert);
    SDL_DestroySurface(imgOriginal);
}

Uint32 Texture::returnPixel(int positionY, int positionX)
{
    return pxlArray2D[positionY][positionX];
}
