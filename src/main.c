// main.c

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gameObject.h"
#include "ivec2.h"
#include "textureGrid.h"
#include "scene.h"  

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define RT_FACTOR 6

#define RT_WIDTH SCREEN_WIDTH/RT_FACTOR
#define RT_HEIGHT SCREEN_HEIGHT/RT_FACTOR


static Scene* scene;

void defUpdater(GameObject* self);
void defDestroyer(GameObject* self);
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

    if(isSceneFull(scene)){
        removeIsland(grid, island);
        return;
        
    }

    IVec2 min;
    IVec2 max;
    TextureGrid* newGrid = newTextureGridFromIsland(island,grid,&min,&max,1);

    removeIsland(grid, island);

    //We want to "offset" the island from the parent object based on how much the min is pushed
    IVec2 offset = {min.x, min.y};

    if(newGrid == NULL){
        return;
    }

    int firstReplaceable = getReplaceableSpot(scene);

    if(firstReplaceable == -1){
        GameObject* obj = newGameObject(parent->position->x + offset.x, parent->position->y + offset.y, 0, 1, 1);
        addEvent(obj, defGridRenderer, Render);
        addEvent(obj, defUpdater, Update);
        addEvent(obj, defDestroyer, Destroy);
        setData(obj, 7, newGrid);
        addObject(scene, obj);
        return;
    }
    else{
        GameObject* obj = scene->objs[firstReplaceable];
        obj->destroyed = false;
        obj->position->x = parent->position->x + offset.x;
        obj->position->y = parent->position->y + offset.y;
        addEvent(obj, defGridRenderer, Render);
        addEvent(obj, defUpdater, Update);
        setData(obj, 7, newGrid);
        addEvent(obj, defDestroyer, Destroy);
       
        scene->count++;
        return;
    }

    
}

void defDestroyer(GameObject* self){
    TextureGrid* grid = (TextureGrid*)getData(self, 7);
    if (grid == NULL){
        return;
    }
    disposeTextureGrid(grid);
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
    if (GetRandomValue(0, 50) < 5 && !IsKeyDown(KEY_LEFT_CONTROL)){
        self->position->x += GetRandomValue(-1, 1);
        self->position->y += GetRandomValue(-1, 1);
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

    //
    Image image = LoadImage("input.png");

    RenderTexture2D target = LoadRenderTexture(RT_WIDTH, RT_HEIGHT);

    Color is[14] = {RED, GOLD, GRAY, LIME, BLUE, VIOLET, BROWN, GREEN, PINK,DARKBROWN, DARKPURPLE, DARKGREEN, DARKGRAY};

    scene = newScene();

    GameObject* g = newGameObject(64,32,0,1,1);
    addEvent(g, defUpdater, Update);
    TextureGrid* grid = newTextureGridFromImage(image);
    setData(g, 7, grid);
    addEvent(g, defGridRenderer, Render);
    addEvent(g, defDestroyer, Destroy);
    addObject(scene, g);

    
    while (!WindowShouldClose())
    {
        // Update logic
        sceneCallEvent(scene, Update);

        if(IsKeyPressed(KEY_R)){
            for(int i = 0; i < scene->capacity; i++){
                //Run a 50/50 chance of destroying the object
                if(scene->objs[i] != NULL && GetRandomValue(0,5) == 1 && scene->objs[i]->destroyed == false){
                    GameObject* obj = scene->objs[i];
                    removeIndex(scene, i);
                    disposeGameObject(obj);
                }
            }
        }

        // Render logic
        BeginDrawing();

            // Start drawing to the render texture
            BeginTextureMode(target);

                ClearBackground(RAYWHITE);


                sceneCallEvent(scene, Render);

                for(int i = 0; i < scene->capacity; i++){
                    if(scene->objs[i] != NULL){
                        //Draw the index
                        

                        if(!scene->objs[i]->destroyed){
                            TextureGrid* grid = (TextureGrid*)getData(scene->objs[i], 7);
                            if(grid != NULL){
                                //Render bounds
                                DrawRectangleLines(scene->objs[i]->position->x, scene->objs[i]->position->y, grid->width, grid->height, DARKBLUE);
                            }
                        }

                        DrawText(TextFormat("%i", i), scene->objs[i]->position->x, scene->objs[i]->position->y, 2, RED);
                    }
                }


            EndTextureMode();

            DrawTexturePro(target.texture, (Rectangle){0, 0, target.texture.width, -target.texture.height}, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);

            //Show mouse position
            DrawText(TextFormat("FPS: %i",GetFPS()), 10, 10, 40, BLACK);

            //Current scene objects
            DrawText(TextFormat("Objects: %i",scene->count), 10, 50, 40, BLACK);
            
        EndDrawing();
    }


}


