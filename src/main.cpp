//main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <raylib.h>

#include "utils.hpp"

#include "gameComponent.hpp"
#include "gameObject.hpp"

#include "gc_render.hpp"
#include "gc_move.hpp"
#include "gc_textureGrid.hpp"


#define ASSET_DIR "./assets/"

using namespace std;

Vector2 lastMousePos = {0,0};
Vector2 lastVMousePos = {0,0};

int screenWidth = 1920;
int screenHeight = 1080;

int main() {

    //Load the version of the game from config/version
    std::ifstream versionFile(ASSET_DIR "config/version");
    if(versionFile.is_open()) {
        std::getline(versionFile, version);
        versionFile.close();
        TraceLog(LOG_INFO, "Version: %s", version.c_str());
        version = version;
    }
    else {
        version = "Unknown";
        TraceLog(LOG_WARNING, "Could not open version file");
    }
    InitWindow(screenWidth, screenHeight, "Raylib Project");

    Image icon = LoadImage(ASSET_DIR "images/icons/mc.png");

    ImageResizeNN(&icon, 256, 256);

    SetWindowIcon(icon);

    UnloadImage(icon);

    GameScene scene;
    currentScene = &scene;

    shared_ptr<GameObject> test = scene.addGameObject(GameObject("Test", {905, 505}, {128*WORLDSCALE, 128*WORLDSCALE}, {64*WORLDSCALE,64*WORLDSCALE}, 0));
    Image image = LoadImage(ASSET_DIR "work.png");
    test->addComponent<TextureGridComponent>(TextureGridComponent(nullptr, image,true));
    test->addComponent<MoveComponent>(MoveComponent(nullptr, 3, false));


    debugMode = false;

    SetTargetFPS(60);
    
    double start = 0;
    double end = 0;
    double elapsedRend = 0;

    while (!WindowShouldClose()) {
        ///Update
        lastMousePos = GetMousePosition();

        scene.update();
        if(IsKeyPressed(KEY_F1)){
            debugMode = !debugMode;
        }

        ///Drawing
        BeginDrawing();

            ClearBackground(BLACK);

            if(debugMode)
                start = GetTime();
            scene.render();
            if(debugMode){
                end = GetTime();
                elapsedRend = end - start;
            }

            scene.renderUI();

            if(debugMode) {
                DrawCircle(lastMousePos.x, lastMousePos.y, 4, RED);
                int FPS = GetFPS();
                std::string fpsText = "FPS : " + std::to_string(FPS);
                DrawText(fpsText.c_str(), 10, 10, 40, RED);
                int objCount = scene.gameObjects.size();
                std::string objText = "Objects : " + std::to_string(objCount);
                DrawText(objText.c_str(), 10, 50, 40, RED);
                std::string timeText = "Render Time : " + std::to_string(elapsedRend) + " \n " + std::to_string(1.0 / 60);
                DrawText(timeText.c_str(), 10, 90, 40, RED);
            }

        EndDrawing();

        //_sleep(1000);

    }

    CloseWindow();

    return 0;
}