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

    shared_ptr<GameObject> test = scene.addGameObject(GameObject("Test", {905, 505}, {16*WORLDSCALE, 16*WORLDSCALE}, {0,0}, 0));
    Image image = LoadImage(ASSET_DIR "work.png");
    test->addComponent<TextureGridComponent>(TextureGridComponent(nullptr, image,true));
    test->addComponent<MoveComponent>(MoveComponent(nullptr, 3, false));

    test->getComponent<TextureGridComponent>().cropSelfToFit();

    debugMode = false;

    SetTargetFPS(120);
    

    while (!WindowShouldClose()) {
        ///Update
        lastMousePos = GetMousePosition();

        scene.update();
        if(IsKeyPressed(KEY_F1)){
            debugMode = !debugMode;
        }

        ///Drawing
        BeginDrawing();

            ClearBackground(WHITE);
            scene.render();

            scene.renderUI();

            if(debugMode) {
                DrawCircle(lastMousePos.x, lastMousePos.y, 4, RED);
                int FPS = GetFPS();
                DrawText(std::to_string(FPS).c_str(), 10, 10, 70, RED);
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}