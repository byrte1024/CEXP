//gc_textureGrid.cpp

#include "gameObject.hpp"
#include "gc_textureGrid.hpp"
#include "gameComponent.hpp"
#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include "utils.hpp"
#include "gc_move.hpp"
#include "gc_render.hpp"
#include <memory>

void TextureGridComponent::render() {
    if(gameObject->readytodie){
        return;
    }
    /*
    Rectangle source = {0, 0, (float)image.width, (float)image.height};
    Rectangle dest = {static_cast<float>(gameObject->getX()), static_cast<float>(gameObject->getY()), (float)gameObject->getW(), (float)gameObject->getH()};
    Vector2 origin = {static_cast<float>(gameObject->getPX()), static_cast<float>(gameObject->getPY())};
    DrawTexturePro(texture, source, dest, origin, gameObject->getRotation(), WHITE);

    Vector2 mp = GetMousePosition();
    mp.x = mp.x / 6;
    mp.y = mp.y / 6;
    Vector2 mousePos = worldToPixel(mp);

    if(isValidPixel(mousePos.x, mousePos.y)) {
        DrawPixel(mousePos.x, mousePos.y, WHITE);

    }

    if(debugMode){
        for(int x = 0; x < image.width; x++) {
            for(int y = 0; y < image.height; y++) {
                DrawPixel(x, y, BLUE);
            }
        }
    }
    */
    Rectangle source = {renderRect.x, renderRect.y, (float)renderRect.width, (float)renderRect.height};
    Rectangle dest = {gameObject->position.x, gameObject->position.y, gameObject->size.x, gameObject->size.y};
    Vector2 origin = gameObject->pivot;

    Texture target = texture;

    DrawTexturePro(target, source, dest, origin, gameObject->rotation, WHITE);

    Vector2 mousePosition = GetMousePosition();
    mousePosition = mousePosition;
    
    std::vector<Color> islandColors = {RED, GREEN, BLUE, YELLOW, ORANGE, PINK, PURPLE, BROWN, BEIGE, LIME, GOLD, SKYBLUE, DARKBLUE, DARKGREEN, DARKPURPLE, DARKBROWN, GRAY, DARKGRAY, LIGHTGRAY, MAROON};

    Vector2 pixelPosition = worldToPixel(mousePosition);

    if(pixelPosition.x < renderRect.x || pixelPosition.x >= renderRect.x + renderRect.width || pixelPosition.y < renderRect.y || pixelPosition.y >= renderRect.y + renderRect.height) {
        return;
    }

    DrawPixel(mousePosition.x, mousePosition.y, WHITE);
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        ImageDrawPixel(&image, pixelPosition.x, pixelPosition.y, RED);
        updateEntireTexture();
        recalculateIslands();
    }
    else if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        ImageDrawPixel(&image, pixelPosition.x, pixelPosition.y, BLANK);
        if(delCheck()){
            gameObject->readytodie = true;
            return;
        }
        cropSelfToFit(false);
        updateEntireTexture();
        recalculateIslands();
    }

    /*
    if(debugMode){

        std::vector<Image> islands = findIslands();
        for(int i = 0; i < islands.size(); i++) {
            for(int x = 0; x < islands[i].width; x++) {
                for(int y = 0; y < islands[i].height; y++) {
                    if(GetImageColor(islands[i], x, y).a != 0) {
                        Vector2 pos = pixelToWorld({static_cast<float>(x), static_cast<float>(y)});
                        DrawRectanglePro({pos.x, pos.y, 6, 6}, {3,3}, gameObject->rotation, {islandColors[i].r,islandColors[i].g,islandColors[i].b,100});
                    }
                }
            }
            UnloadImage(islands[i]);
        }

    }
        */

}

void TextureGridComponent::update() {
    //Get the mouse position
    
}

void TextureGridComponent::start() {
    if(!norescale){
        gameObject->size = {static_cast<float>(image.width * WORLDSCALE), static_cast<float>(image.height * WORLDSCALE)};
    }
    //print the size
    std::cout << "Size: " << gameObject->size.x << ", " << gameObject->size.y << std::endl;
    calcPivot();
}

void TextureGridComponent::renderUI() {

    /*
    //Draw each pixel as a rectangle
    for(int x = 0; x < image.width; x++) {
        for(int y = 0; y < image.height; y++) {
            //Check if alpha is not equal to 0
            if(GetImageColor(image, x, y).a != 0) {
                //Draw a rectangle
                Vector2 pos = pixelToWorld({static_cast<float>(x), static_cast<float>(y)});

                

                DrawRectanglePro({pos.x*6, pos.y*6, 3,3}, {-1.5f,-1.5f}, gameObject->rotation, {0,0,255,100});
            }
        }
    }
    */
}

//Updates the texture / uploads it to the GPU to represent the image
void TextureGridComponent::updateEntireTexture() {
    if(delay > 0) {
        return;
    }
    //texture.width = image.width;
    //texture.height = image.height;
    UpdateTexture(texture, image.data);
}

void TextureGridComponent::createTexture() {
    texture = LoadTextureFromImage(image);
}

void TextureGridComponent::recalculateIslands() {
    // Create a vector to store the islands.
    std::vector<Image> islands;

    islands = findIslands();

    for(int i = 0; i < islands.size(); i++) {
        if(i!=0){
            seperateIsland(islands[i]);
        }
        else{
            UnloadImage(islands[i]);
        }
    }
}

void TextureGridComponent::removeIsland(Image island) {
    for(int x = 0; x < image.width; x++) {
        for(int y = 0; y < image.height; y++) {
            if(GetImageColor(island, x, y).a != 0) {
                ImageDrawPixel(&image, x, y, BLANK);
            }
        }
    }
}

bool TextureGridComponent::delCheck(){
    for(int x = 0; x < image.width; x++) {
        for(int y = 0; y < image.height; y++) {
            if(GetImageColor(image, x, y).a != 0) {
                return false;
            }
        }
    }
    return true;
}


void TextureGridComponent::seperateIsland(Image island) {

    removeIsland(island);

    /*
    //Account for rotation of ourselves
    Vector2 trueOffset = rotateVector(offset, gameObject->rotation);

    TextureGridComponent tg = TextureGridComponent(nullptr, croppedIsland, true);
    
    Vector2 pos = gameObject->position + (trueOffset * WORLDSCALE * 1);
    Vector2 size = { (float)croppedIsland.width * WORLDSCALE, (float)croppedIsland.height * WORLDSCALE };

    // Check the world position of our 0,0
    Vector2 zerozeroold = pixelToWorld({0, 0}) / WORLDSCALE ;

    shared_ptr<GameObject> newIsland = currentScene->addGameObject(GameObject("Island", pos, size, {0,0}, gameObject->rotation));
    newIsland->addComponent<TextureGridComponent>(tg);
    newIsland->addComponent<MoveComponent>(gameObject->getComponent<MoveComponent>());

    // Check the world position of our 0,0
    Vector2 zerozero = newIsland->getComponent<TextureGridComponent>().pixelToWorld({0, 0}) / WORLDSCALE;

    //The difference between the two should be the offset (zerozeroold-zerozer-offset), whatever leftover should be applied to position
    Vector2 diff = (zerozeroold - zerozero + trueOffset) * WORLDSCALE;

    newIsland->position = newIsland->position + diff;

    */

    shared_ptr<GameObject> newIsland = currentScene->addGameObject(GameObject("Island", gameObject->position, gameObject->size, {0,0}, gameObject->rotation));
    TextureGridComponent& tg = newIsland->addComponent<TextureGridComponent>(gameObject->getComponent<TextureGridComponent>());
    tg.image = island;
    tg.createTexture();
    tg.cropSelfToFit();
    newIsland->addComponent<MoveComponent>(gameObject->getComponent<MoveComponent>());
    cropSelfToFit();
    updateEntireTexture();
}

void TextureGridComponent::cropSelfToFit(bool force) {

    //Find the bounds of renderrect (the part of the image we are rendering, NOT THE IMAGE ITSELF!)
    int minX = image.width;
    int minY = image.height;

    int maxX = 0;
    int maxY = 0;

    for(int x = 0; x < image.width; x++) {
        for(int y = 0; y < image.height; y++) {
            if(GetImageColor(image, x, y).a != 0) {
                if(x < minX) {
                    minX = x;
                }
                if(y < minY) {
                    minY = y;
                }
                if(x > maxX) {
                    maxX = x;
                }
                if(y > maxY) {
                    maxY = y;
                }
            }
        }
    }

   
    
    

    Vector2 offset  = {minX - renderRect.x, minY - renderRect.y};

    renderRect = {static_cast<float>(minX), static_cast<float>(minY), static_cast<float>(maxX - minX + 1), static_cast<float>(maxY - minY + 1)};

    Vector2 trueOffset = rotateVector(offset, gameObject->rotation);
    // Update our transformation
    gameObject->size = {static_cast<float>(renderRect.width * WORLDSCALE), static_cast<float>(renderRect.height * WORLDSCALE)};
    //gameObject->position = gameObject->position + trueOffset * WORLDSCALE;

    calcPivot();
    
    Vector2 oldzz = pixelToWorld({0, 0});

    //Check if center pixel is outside render rect
    if(centerPixel.x < renderRect.x || centerPixel.x > renderRect.x + renderRect.width || centerPixel.y < renderRect.y || centerPixel.y > renderRect.y + renderRect.height){
        centerPixel = {floor(static_cast<float>(renderRect.width)/2 + renderRect.x), floor(static_cast<float>(renderRect.height)/2  + renderRect.y)};
    }

    calcPivot();

    Vector2 newzz = pixelToWorld({0, 0});

    Vector2 diff = (oldzz - newzz);

    gameObject->position = gameObject->position + diff;

}

Vector2 TextureGridComponent::pixelToWorld(Vector2 pixelPos) {
    // Step 1: Adjust the pixel position based on the renderRect offset
    Vector2 adjustedPixelPos = { pixelPos.x - renderRect.x, pixelPos.y - renderRect.y };

    // Step 2: Convert from pixel coordinates to world units.
    Vector2 worldUnits = { adjustedPixelPos.x * WORLDSCALE, adjustedPixelPos.y * WORLDSCALE };

    // Step 3: Remove the pivot offset.
    Vector2 unpivoted = { worldUnits.x - (gameObject->pivot.x),
                          worldUnits.y - gameObject->pivot.y };

    // Step 4: Apply the object's rotation (rotate by +rotation).
    float rad = gameObject->rotation * DEG2RAD;
    float cosTheta = cos(rad);
    float sinTheta = sin(rad);
    Vector2 rotated = { unpivoted.x * cosTheta - unpivoted.y * sinTheta,
                        unpivoted.x * sinTheta + unpivoted.y * cosTheta };

    // Step 5: Translate back to world coordinates.
    Vector2 worldPos = { rotated.x + gameObject->position.x,
                         rotated.y + gameObject->position.y };

    return worldPos;
}


Vector2 TextureGridComponent::worldToPixel(Vector2 worldPos) {
    // Step 1: Translate the world coordinate into the object's local space.
    Vector2 localPos = { worldPos.x - gameObject->position.x,
                         worldPos.y - gameObject->position.y };

    // Step 2: Apply the inverse rotation (rotate by -rotation).
    // Convert rotation to radians.
    float rad = -gameObject->rotation * DEG2RAD;
    float cosTheta = cos(rad);
    float sinTheta = sin(rad);
    Vector2 unrotated = { localPos.x * cosTheta - localPos.y * sinTheta,
                          localPos.x * sinTheta + localPos.y * cosTheta };

    // Step 3: Add the pivot offset.
    Vector2 pixelPos = { unrotated.x + (gameObject->pivot.x),
                         unrotated.y + (gameObject->pivot.y) };

    // Step 4: Convert from world units to pixel coordinates (if WORLDSCALE != 1).
    pixelPos.x = floor(pixelPos.x / WORLDSCALE);
    pixelPos.y = floor(pixelPos.y / WORLDSCALE);

    // Step 5: Adjust the pixel position based on the renderRect offset
    pixelPos.x += renderRect.x;
    pixelPos.y += renderRect.y;

    return pixelPos;
}


void disposeIslands(std::vector<Image>& islands) {
    for (Image& island : islands) {
        UnloadImage(island);
    }
}
constexpr int MAX_STACK_SIZE = 100000; // Adjust as needed for large images
void TextureGridComponent::floodFill(Vector2 pixel, bool* visited, std::vector<Image>& islands) {
    if (!isValidPixel(pixel) || visited[static_cast<int>(pixel.y) * image.width + static_cast<int>(pixel.x)] || isTransparentPixel(pixel)) {
        return;
    }

    // Create an image for this island
    Image island = GenImageColor(image.width, image.height, BLANK);

    // Preallocated stack for DFS (using a fixed-size array instead of std::stack)
    
    Vector2 stack[MAX_STACK_SIZE];
    int stackIndex = 0;

    // Push the initial pixel onto the stack
    stack[stackIndex++] = pixel;

    // Start flood fill using iterative DFS
    while (stackIndex > 0) {
        Vector2 current = stack[--stackIndex]; // Pop from stack

        int index = current.y * image.width + current.x;
        if (visited[index] || isTransparentPixel(current)) {
            continue;
        }

        // Mark as visited
        visited[index] = true;

        // Copy pixel to the island image
        ImageDrawPixel(&island, current.x, current.y, GetImageColor(image, current.x, current.y));

        // Push neighboring pixels onto the stack (manual inlining for better performance)
        if (current.x > 0 && !visited[index - 1]) stack[stackIndex++] = {current.x - 1, current.y};
        if (current.x < image.width - 1 && !visited[index + 1]) stack[stackIndex++] = {current.x + 1, current.y};
        if (current.y > 0 && !visited[index - image.width]) stack[stackIndex++] = {current.x, current.y - 1};
        if (current.y < image.height - 1 && !visited[index + image.width]) stack[stackIndex++] = {current.x, current.y + 1};
    }

    // Store the island
    islands.push_back(island);
}


std::vector<Image> TextureGridComponent::findIslands() {
    std::vector<Image> islands;

    // Create a visited array
    bool* visited = new bool[image.width * image.height]{};

    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            if (!visited[y * image.width + x] && !isTransparentPixel({static_cast<float>(x), static_cast<float>(y)})) {
                floodFill({static_cast<float>(x), static_cast<float>(y)}, visited, islands);
            }
        }
    }

    delete[] visited;

    return islands;
}

void TextureGridComponent::calcPivot(){
    //Find the position of the center pixel and multiply it by world size
    Vector2 centerPos = {(centerPixel.x-renderRect.x) * WORLDSCALE, (centerPixel.y-renderRect.y) * WORLDSCALE};


    //Set that to our game object
    gameObject->pivot = centerPos;
}

void TextureGridComponent::onDestroy() {
    UnloadImage(image);
    UnloadTexture(texture);
}

Vector2 TextureGridComponent::true00(){ 
    return (pixelToWorld({-renderRect.x, -renderRect.y}) / WORLDSCALE);
}