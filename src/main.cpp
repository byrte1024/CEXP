//main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "win.hpp"

#include "raylib.h"

#include <iomanip> // For controlling decimal precision
#include <sstream> // For std::ostringstream
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

int fps = 120;

int screenWidth = 1920;
int screenHeight = 1080;

//(*TraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages

int main() {


    SetTraceLogCallback([](int logLevel, const char *text, va_list args) {
        string logText = "";
        switch (logLevel) {
            case LOG_ALL:
                logText = "ALL: ";
                break;
            case LOG_TRACE:
                logText = "TRACE: ";
                break;
            case LOG_DEBUG:
                logText = "DEBUG: ";
                break;
            case LOG_INFO:
                logText = "INFO: ";
                break;
            case LOG_WARNING:
                logText = "WARNING: ";
                break;
            case LOG_ERROR:
                logText = "ERROR: ";
                break;
            case LOG_FATAL:
                logText = "FATAL: ";
                break;
            case LOG_NONE:
                logText = "NONE: ";
                break;
        }
        logText += text;
        vprintf(logText.c_str(), args);
        printf("\n");

        if(logLevel == LOG_ERROR || logLevel == LOG_FATAL) {
            //Parse the string with args
            char buffer[1024];
            vsprintf(buffer, text, args);
            errorPopup("Error", buffer);
        }
        if(logLevel == LOG_WARNING) {
            //Parse the string with args
            char buffer[1024];
            vsprintf(buffer, text, args);
            warningPopup("Warning", buffer);
        }
        if(logLevel == LOG_DEBUG) {
            //Parse the string with args
            char buffer[1024];
            vsprintf(buffer, text, args);
            messagePopup("Debug", buffer);
        }

    });

    generate_lookup_table();
    //print_lookup_table();

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

    SetTargetFPS(fps);
    
    double start = 0;
    double end = 0;
    double elapsedRend = 0;
    double elapsedLogic = 0;

    while (!WindowShouldClose()) {
        ///Update
        lastMousePos = GetMousePosition();

        if(debugMode)
            start = GetTime();
        scene.update();
        if(debugMode){
            end = GetTime();
            elapsedLogic = end - start;
        }
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


            if (debugMode) { 
                // Draw Circle (keep it as is for debugging mouse position)
                DrawCircle(lastMousePos.x, lastMousePos.y, 4, RED);
                
                // FPS Calculation
                int FPS = GetFPS();
                float total = 1.0f / FPS;
            
                // Game object count
                int objCount = scene.gameObjects.size();
            
                // Calculating render, logic, and sleep percentages
                float rend = elapsedRend;
                float logic = elapsedLogic;
                float sleep = total - rend - logic;
            
                // Round the values to one decimal place
                rend = round(rend / total * 1000) / 10;
                logic = round(logic / total * 1000) / 10;
                sleep = round(sleep / total * 1000) / 10;
            
                // Format the percentages to show only one decimal point
                std::ostringstream rendStream, logicStream, sleepStream;
                rendStream << std::fixed << std::setprecision(1) << rend;
                logicStream << std::fixed << std::setprecision(1) << logic;
                sleepStream << std::fixed << std::setprecision(1) << sleep;

                int sceneCapacity = scene.gameObjects.capacity();
            
                // Create a single string with all debug info
                std::string debugInfo = 
                    "FPS: " + std::to_string(FPS) + "\n" +
                    "Objects: " + std::to_string(objCount) + " / " + std::to_string(sceneCapacity) + "\n" +
                    "LT: " + std::to_string(elapsedLogic) + "\n" +
                    "Rend: " + rendStream.str() + "%\n" +
                    "Logic: " + logicStream.str() + "%\n" +
                    "Sleep: " + sleepStream.str() + "%";
            
                // Draw the consolidated debug text in one call
                DrawText(debugInfo.c_str(), 10, 10, 40, RED);
            }
            
            

        EndDrawing();

        //_sleep(1000);

    }

    CloseWindow();

    return 0;
}