//textureGrid.h

#ifndef TEXTUREGRID_H
#define TEXTUREGRID_H

#include <raylib.h>
#include <stdlib.h>
#include "ivec2.h"

#define MAX_ISLANDS 32



typedef struct TextureGrid {
    Image image;
    Texture2D texture;
    int width;
    int height;
} TextureGrid;

//Create a new TextureGrid object
TextureGrid* newTextureGrid(int width, int height);

//Create a new TextureGrid object from an existing texture
TextureGrid* newTextureGridFromImage(Image texture);

//Dispose of a TextureGrid object
void disposeTextureGrid(TextureGrid *textureGrid);

//Get the next power of two of a number
int nextPowerOfTwo(int n);

//Convert a 2D vector to a 1D index based on the width and height of the grid using bitshifting
int vectorToIndex(IVec2 v, int width, int height);


//Set the color of a pixel in the grid
void setPixelColor(TextureGrid *textureGrid,IVec2 v, Color color);

//Get the color of a pixel in the grid
Color getPixelColor(TextureGrid *textureGrid, IVec2 v);



//Update a pixel in the grid to the texture
void updatePixel(TextureGrid *textureGrid, IVec2 v);

//Update a range of pixels in the grid to the texture (x1, y1) to (x2, y2) (both are inclusive)
void updatePixelRange(TextureGrid *textureGrid, IVec2 v1, IVec2 v2);

//Update a list of pixels in the grid to the texture
void updatePixelList(TextureGrid *textureGrid, IVec2 *pixels, int count);

//Update the entire texture (WARNING! Try not to use this too much, it's slow)
void updateTexture(TextureGrid *textureGrid);


typedef struct Island {
    IVec2* pixels;
    int pixelCount;
} Island;

Island* newIsland(IVec2* pixels, int pixelCount);

void disposeIsland(Island* island);

//Flood fill
void floodFill(TextureGrid *textureGrid, bool *visited, IVec2 start, IVec2 *islandPixels, int *pixelCount);

//Return a list of "islands" as images
void findIslands(TextureGrid *textureGrid, int* islandCount, Island* islands);

//Remove an island from the grid
void removeIsland(TextureGrid *textureGrid, Island* island);

//Find the bounds of an island
void findIslandBounds(Island* island, IVec2* min, IVec2* max);

//Create a new texture grid using an island
TextureGrid* newTextureGridFromIsland(Island* island, TextureGrid* original, IVec2* minOUT, IVec2* maxOUT, int minSize);

#endif