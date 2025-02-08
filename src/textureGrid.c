//textureGrid.c

#include "textureGrid.h"
#include <raylib.h>
#include "ivec2.h"
#include <stdlib.h>
#include <stdio.h>


inline int nextPowerOfTwo(int n) {
    return 1 << (32 - __builtin_clz(n - 1));
}

//Convert a 2D vector to a 1D index based on the width and height of the grid using bitshifting
inline int vectorToIndex(IVec2 v, int width, int height) {
    /*
    int maxSize = (width > height) ? width : height;
    int powerOfTwo = nextPowerOfTwo(maxSize);
    return (v.x << __builtin_ctz(powerOfTwo)) | v.y;
    */

    //Simpler but less effecient solution
    return v.x + v.y * width;
}


TextureGrid* newTextureGrid(int width, int height){
    Image texture = GenImageColor(width, height, BLANK);
    return newTextureGridFromImage(texture);
}

TextureGrid* newTextureGridFromImage(Image image){
    TextureGrid* textureGrid = malloc(sizeof(TextureGrid));

    textureGrid->texture = LoadTextureFromImage(image);

    textureGrid->width = image.width;
    textureGrid->height = image.height;

    textureGrid->image = image;


    return textureGrid;
}

void disposeTextureGrid(TextureGrid *textureGrid){
    UnloadTexture(textureGrid->texture);
    free(textureGrid);
}

void setPixelColor(TextureGrid *textureGrid, IVec2 v, Color color){
    ImageDrawPixel(&textureGrid->image, v.x, v.y, color);

    updatePixel(textureGrid, v);
}

Color getPixelColor(TextureGrid *textureGrid, IVec2 v){
    return GetImageColor(textureGrid->image, v.x, v.y);
}

void updatePixel(TextureGrid *textureGrid, IVec2 v){
    updateTexture(textureGrid);
}

void updatePixelRange(TextureGrid *textureGrid, IVec2 v1, IVec2 v2){
    updateTexture(textureGrid);
}

void updatePixelList(TextureGrid *textureGrid, IVec2 *pixels, int count){
    updateTexture(textureGrid);
}

void updateTexture(TextureGrid *textureGrid){
    UpdateTexture(textureGrid->texture, textureGrid->image.data);
}

// Flood fill algorithm to find all connected pixels not transparent pixels
void floodFill(TextureGrid *textureGrid, bool *visited, IVec2 start, IVec2 *islandPixels, int *pixelCount) {
    int width = textureGrid->width;
    int height = textureGrid->height;

    // Allocate queue with a dynamic size management approach
    int queueCapacity = width * height;
    IVec2 *queue = malloc(queueCapacity * sizeof(IVec2));

    if (!queue) return; // Ensure memory allocation success
    
    int front = 0, back = 0;
    
    // Mark the start pixel as visited and enqueue it
    visited[vectorToIndex(start, width, height)] = true;
    queue[back++] = start;

    while (front < back) {
        IVec2 current = queue[front++]; // Dequeue efficiently
        islandPixels[(*pixelCount)++] = current;


        // Define neighbors (right, left, down, up)
        IVec2 neighbors[4] = {
            {current.x + 1, current.y},
            {current.x - 1, current.y},
            {current.x, current.y + 1},
            {current.x, current.y - 1}
        };
        
        for (int i = 0; i < 4; i++) {
            IVec2 neighbor = neighbors[i];
        
            // Bounds check
            if (neighbor.x >= 0 && neighbor.x < width && neighbor.y >= 0 && neighbor.y < height) {
                int index = vectorToIndex(neighbor, width, height);
                
                // Check if not visited and is not transparent
                if (!visited[index] && getPixelColor(textureGrid, neighbor).a != 0) {
                    visited[index] = true;
                    queue[back++] = neighbor;
                }
            }
        }
    }
    
    free(queue);
}

Island* newIsland(IVec2* pixels, int pixelCount){
    Island* island = malloc(sizeof(Island));
    island->pixels = pixels;
    island->pixelCount = pixelCount;
    return island;
}

void disposeIsland(Island* island){
    free(island->pixels);
    free(island);
}

void findIslands(TextureGrid *textureGrid, int* islandCount, Island* islands){
    int width = textureGrid->width;
    int height = textureGrid->height;

    // If width or height is 0, return
    if (width < 1 || height < 1) return;

    // Allocate visited array with a dynamic size management approach
    bool *visited = malloc(width * height * sizeof(bool));

    if (visited==NULL) return; // Ensure memory allocation success
    
    // Initialize visited array
    for (int i = 0; i < width * height; i++) visited[i] = false;
    
    // Initialize island count
    *islandCount = 0;
    
    // Iterate all pixels to find islands
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            IVec2 current = {x, y};
            int index = vectorToIndex(current, width, height);
            
            // Check if not visited and is not transparent
            if (!visited[index] && getPixelColor(textureGrid, current).a != 0) {
                int pixelCount = 0;
                IVec2 *islandPixels = malloc(width * height * sizeof(IVec2));
                floodFill(textureGrid, visited, current, islandPixels, &pixelCount);
                
                // Create new island
                islands[*islandCount] = (Island){islandPixels, pixelCount};
                (*islandCount)++;
            }
        }
    }
    
    free(visited);
}

//Remove an island from the grid
void removeIsland(TextureGrid *textureGrid, Island* island){
    for (int i = 0; i < island->pixelCount; i++){
        setPixelColor(textureGrid, island->pixels[i], BLANK);
    }
}

//Find the bounds of an island
void findIslandBounds(Island* island, IVec2* min, IVec2* max){
    min->x = INT_MAX;
    min->y = INT_MAX;
    max->x = INT_MIN;
    max->y = INT_MIN;

    for (int i = 0; i < island->pixelCount; i++){
        IVec2 pixel = island->pixels[i];
        if (pixel.x < min->x){
            min->x = pixel.x;
        }
        if (pixel.y < min->y){
            min->y = pixel.y;
        }
        if (pixel.x > max->x){
            max->x = pixel.x;
        }
        if (pixel.y > max->y){
            max->y = pixel.y;
        }
    }
}

#define MIN_SPLIT_SIZE 1

//Create a new texture grid using an island
TextureGrid* newTextureGridFromIsland(Island* island, TextureGrid* original, IVec2* minOUT, IVec2* maxOUT, int minSize){
    IVec2 min;
    IVec2 max;
    findIslandBounds(island, &min, &max);

    //Check if the island is empty
    if (min.x == INT_MAX){
        return NULL;
    }

    int width = max.x - min.x + 1;
    int height = max.y - min.y + 1;

    //Check if the island is too small
    if(width <= minSize && height <= minSize){
        return NULL;
    }

    Image texture = GenImageColor(width, height, BLANK);

    for (int i = 0; i < island->pixelCount; i++){
        IVec2 pixel = island->pixels[i];
        ImageDrawPixel(&texture, pixel.x - min.x, pixel.y - min.y, getPixelColor(original, pixel));
    }

    minOUT->x = min.x;
    minOUT->y = min.y;

    return newTextureGridFromImage(texture);
}



