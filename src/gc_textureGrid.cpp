//gc_textureGrid.cpp

#include "gameObject.hpp"
#include "gc_textureGrid.hpp"
#include "gameComponent.hpp"
#include <iostream>
#include <raylib.h>
#include "utils.hpp"
#include "gc_move.hpp"
#include "gc_render.hpp"

void TextureGridComponent::render() {
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
    Rectangle source = {0, 0, (float)image.width, (float)image.height};
    Rectangle dest = gameObject->getRect();
    Vector2 origin = gameObject->pivot;

    DrawTexturePro(texture, source, dest, origin, gameObject->rotation, WHITE);

    Vector2 mousePosition = GetMousePosition();
    mousePosition = mousePosition;
    
    std::vector<Color> islandColors = {RED, GREEN, BLUE, YELLOW, ORANGE, PINK, PURPLE, BROWN, BEIGE, LIME, GOLD, SKYBLUE, DARKBLUE, DARKGREEN, DARKPURPLE, DARKBROWN, GRAY, DARKGRAY, LIGHTGRAY, MAROON};

    Vector2 pixelPosition = worldToPixel(mousePosition);

    if(isValidPixel(pixelPosition)) {
        DrawPixel(mousePosition.x, mousePosition.y, WHITE);
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            ImageDrawPixel(&image, pixelPosition.x, pixelPosition.y, RED);
            updateEntireTexture();
            recalculateIslands();
        }
        else if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            ImageDrawPixel(&image, pixelPosition.x, pixelPosition.y, BLANK);
            updateEntireTexture();
            recalculateIslands();
        }
    }

    //Draw at center pixel
    Vector2 centerPos = pixelToWorld(centerPixel);
    DrawRectanglePro({centerPos.x, centerPos.y, 6, 6}, {3,3}, gameObject->rotation, RED);

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
        gameObject->size = {static_cast<float>(image.width*WORLDSCALE), static_cast<float>(image.height*WORLDSCALE)};
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
        UnloadImage(islands[i]);
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
    updateEntireTexture();
}

Image TextureGridComponent::cropImageToFit(Image island, Vector2& offset) {
    int minX = island.width, minY = island.height, maxX = 0, maxY = 0;
    
    // Find the bounding box of the non-transparent pixels
    for (int x = 0; x < island.width; x++) {
        for (int y = 0; y < island.height; y++) {
            if (GetImageColor(island, x, y).a != 0) {
                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;
            }
        }
    }
    
    // Ensure the bounds are valid
    if (maxX < minX || maxY < minY) return GenImageColor(1, 1, BLANK);
    
    // Set the offset to update the new position
    offset = { (float)minX, (float)minY };
    
    // Crop the image to the bounding box
    Rectangle cropRect = { (float)minX, (float)minY, (float)(maxX - minX + 1), (float)(maxY - minY + 1) };
    Image croppedImage = ImageFromImage(island, cropRect);
    
    return croppedImage;
}


void TextureGridComponent::seperateIsland(Image island) {
    Vector2 offset;
    Image croppedIsland = cropImageToFit(island, offset);
    removeIsland(island);

    printf("Offset: %f, %f\n", offset.x, offset.y);

    //Account for rotation of ourselves
    Vector2 trueOffset = rotateVector(offset, gameObject->rotation);

    TextureGridComponent tg = TextureGridComponent(nullptr, croppedIsland, true);
    
    Vector2 pos = gameObject->position + (trueOffset * WORLDSCALE * 1);
    Vector2 size = { (float)croppedIsland.width * WORLDSCALE, (float)croppedIsland.height * WORLDSCALE };

    // Check the world position of our 0,0
    Vector2 zerozeroold = pixelToWorld({0, 0}) / WORLDSCALE ;

    shared_ptr<GameObject> newIsland = currentScene->addGameObject(GameObject("Island", pos, size, {0,0}, gameObject->rotation));
    newIsland->addComponent<TextureGridComponent>(tg);
    newIsland->addComponent<MoveComponent>(MoveComponent(nullptr, 3, false));

    // Check the world position of our 0,0
    Vector2 zerozero = newIsland->getComponent<TextureGridComponent>().pixelToWorld({0, 0}) / WORLDSCALE;

    //The difference between the two should be the offset (zerozeroold-zerozer-offset), whatever leftover should be applied to position
    Vector2 diff = (zerozeroold - zerozero + trueOffset) * WORLDSCALE;

    newIsland->position = newIsland->position + diff;

}


//Converts a pixel position to a world position
Vector2 TextureGridComponent::pixelToWorld(Vector2 pixelPos) {
    // Step 1: Convert from pixel coordinates to world units.
    Vector2 worldUnits = { pixelPos.x * WORLDSCALE, pixelPos.y * WORLDSCALE };
    
    // Step 2: Remove the pivot offset.
    Vector2 unpivoted = { worldUnits.x - (gameObject->pivot.x),
                          worldUnits.y - gameObject->pivot.y};

    // Step 3: Apply the object's rotation (rotate by +rotation).
    float rad = gameObject->rotation * DEG2RAD;
    float cosTheta = cos(rad);
    float sinTheta = sin(rad);
    Vector2 rotated = { unpivoted.x * cosTheta - unpivoted.y * sinTheta,
                        unpivoted.x * sinTheta + unpivoted.y * cosTheta };

    // Step 4: Translate back to world coordinates.
    Vector2 worldPos = { rotated.x + gameObject->position.x,
                         rotated.y + gameObject->position.y };
    
    return worldPos;
}

//Converts a world position to a pixel position
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
    
    return pixelPos;
}

void disposeIslands(std::vector<Image>& islands) {
    for (Image& island : islands) {
        UnloadImage(island);
    }
}

void TextureGridComponent::floodFill(Vector2 pixel, std::vector<std::vector<bool>>& visited, std::vector<Image>& islands) {
    // Check if the pixel is valid and has not been visited.
    if (!isValidPixel(pixel) || visited[pixel.x][pixel.y]) {
        return;
    }

    if (isTransparentPixel(pixel)) {
        return;
    }

    // Create a queue to store pixels to visit.
    std::vector<Vector2> queue;

    // Push the starting pixel onto the queue.
    queue.push_back(pixel);

    // Create an image to store the island.
    Image island = GenImageColor(image.width, image.height, BLANK);

    // While the queue is not empty.
    while (!queue.empty()) {
        // Get the pixel at the front of the queue.
        Vector2 current = queue.front();
        queue.erase(queue.begin());

        // Check if the pixel is valid and has not been visited.
        if (!isValidPixel(current) || visited[current.x][current.y]) {
            continue;
        }

        if (isTransparentPixel(current)) {
            continue;
        }

        // Mark the pixel as visited.
        visited[current.x][current.y] = true;

        // Set the pixel in the island image to the one in the original image.
        ImageDrawPixel(&island, current.x, current.y, GetImageColor(image, current.x, current.y));

        // Add the pixel to the island.
        //island.push_back(current);

        // Add the neighbors to the queue.
        queue.push_back({current.x - 1, current.y});
        queue.push_back({current.x + 1, current.y});
        queue.push_back({current.x, current.y - 1});
        queue.push_back({current.x, current.y + 1});
    }

    // Add the island to the list of islands.
    islands.push_back(island);

}

std::vector<Image> TextureGridComponent::findIslands() {
    // Create a vector to store the islands.
    std::vector<Image> islands;

    // Create a vector to store visited pixels.
    std::vector<std::vector<bool>> visited(image.width, std::vector<bool>(image.height, false));

    // Loop through each pixel in the image.
    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            // Check if the pixel has not been visited.
            if (!visited[x][y]) {
                // Flood fill the island.
                Vector2 pixel = {x, y};
                floodFill(pixel, visited, islands);
            }
        }
    }

    return islands;
}

void TextureGridComponent::calcPivot(){
    //Find the position of the center pixel and multiply it by world size
    Vector2 centerPos = {centerPixel.x * WORLDSCALE, centerPixel.y * WORLDSCALE};

    //Set that to our game object
    gameObject->pivot = centerPos;
}