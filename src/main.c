// main.c

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gameObject.h"
#include "ivec2.h"
#include "textureGrid.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define RT_FACTOR 12

#define RT_WIDTH SCREEN_WIDTH/RT_FACTOR
#define RT_HEIGHT SCREEN_HEIGHT/RT_FACTOR

#define MAX_OBJECTS 255

GameObject* objs[MAX_OBJECTS];
int count = 0;

void defUpdater(GameObject* self);
void defGridRenderer(GameObject* self);

bool inBounds(int x, int y, int width, int height){
    return x >= 0 && x < width && y >= 0 && y < height;
}

void defGridRenderer(GameObject* self){
    //Check if the data is a texture grid
    TextureGrid* grid = (TextureGrid*)getData(self, 7);
    if (grid == NULL){
        return;
    }

    //Draw the texture grid
    DrawTextureEx(grid->texture, (Vector2){self->position->x,self->position->y}, self->rotation, self->scale->x, WHITE);
}

#define MIN_ISLAND_SIZE 1

void seperateIsland(GameObject* parent, Island* island, TextureGrid* grid){
    if(count >= MAX_OBJECTS){
        removeIsland(grid, island);
        return;
        
    }

    IVec2 min;
    IVec2 max;
    TextureGrid* newGrid = newTextureGridFromIsland(island,grid,&min,&max,2);
    removeIsland(grid, island);

    //We want to "offset" the island from the parent object based on how much the min is pushed
    IVec2 offset = {min.x, min.y};

    if(newGrid == NULL){
        return;
    }

    printf("Island %ix%i\n", newGrid->width, newGrid->height);
    GameObject* obj = newGameObject(parent->position->x + offset.x, parent->position->y + offset.y, 0, 1, 1);
    addEvent(obj, defGridRenderer, Render);
    addEvent(obj, defUpdater, Update);
    setData(obj, 7, newGrid);
    objs[count] = obj;
    count++;
    /*
    
    
    
    */
}

void defUpdater(GameObject* self){


    TextureGrid* grid = (TextureGrid*)getData(self, 7);
    if (grid == NULL){
        return;
    }

    Vector2 mousePos = GetMousePosition();
    int vspx = (int)((int)mousePos.x / RT_FACTOR);
    int vspy = (int)((int)mousePos.y / RT_FACTOR);


    int relx = vspx - self->position->x;
    int rely = vspy - self->position->y;


    if(inBounds(relx, rely, grid->width, grid->height)){

        IVec2 v = {relx, rely};
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            setPixelColor(grid, v, RED);
        }
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
            setPixelColor(grid, v, BLANK);
        }
    }

    //Move around randomly
    if (GetRandomValue(0, 500) < 5){
        self->position->x += GetRandomValue(-3, 3);
        self->position->y += GetRandomValue(-3, 3);
    }


    Island* islands = (Island*)malloc(sizeof(Island) * MAX_ISLANDS);
    if(islands == NULL){
        printf("Memory allocation failed");
        return;
    }
    int islandCount = 0;

    findIslands(grid, &islandCount, islands);

    for(int i = 0; i < islandCount; i++){
        if(i!=0){
            seperateIsland(self, &islands[i], grid);
        }
        free(islands[i].pixels);
    }

    free(islands);

    

}


void main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib");

    SetTargetFPS(60);  

    /*
    GameObject* obj = newGameObject(50,50,0,20,20);
    addEvent(obj, defRenderer, Render);
    addEvent(obj, defUpdater, Update);
    */

    Image image = GenImageColor(32, 32, BLACK);

    RenderTexture2D target = LoadRenderTexture(RT_WIDTH, RT_HEIGHT);

    Color is[MAX_OBJECTS] = {RED, GOLD, GRAY, LIME, BLUE, VIOLET, BROWN, GREEN, PINK,DARKBROWN, DARKPURPLE, DARKGREEN, DARKGRAY};

    

    

    objs[count] = newGameObject(64,32,0,1,1);
    addEvent(objs[count], defUpdater, Update);
    TextureGrid* grid = newTextureGridFromImage(image);
    setData(objs[count], 7, grid);
    addEvent(objs[count], defGridRenderer, Render);
    count++;

    
    while (!WindowShouldClose())
    {
        

        for (int i = 0; i < count; i++)
        {
            callEvent(objs[i], Update);
        }
        


        // Render logic
        BeginDrawing();

            // Start drawing to the render texture
            BeginTextureMode(target);

                ClearBackground(RAYWHITE);

                for (int i = 0; i < count; i++)
                {
                    callEvent(objs[i], Render);
                }

            EndTextureMode();

            DrawTexturePro(target.texture, (Rectangle){0, 0, target.texture.width, -target.texture.height}, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);

            //Show mouse position
            DrawText(TextFormat("FPS: %i",GetFPS()), 10, 10, 40, BLACK);

            //Current scene objects
            DrawText(TextFormat("Objects: %i",count), 10, 50, 40, BLACK);
            
        EndDrawing();
    }


}


