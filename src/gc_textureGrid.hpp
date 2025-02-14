//gc_textureGrid.hpp

#ifndef GC_TEXTUREGRID_H 
#define GC_TEXTUREGRID_H

#include <raylib.h>
#include <vector>
#include <iostream> 
#include <cmath>

#include "gameObject.hpp"
#include "gameComponent.hpp"
#include "utils.hpp"

#define WORLDSCALE 6.0f
#define MIN_CROP_SIZE 0.8f

class TextureGridComponent : public GameComponent {
    public:
        //The actual pixel image data
        Image image;

        bool norescale = false;

        Vector2 centerPixel = {0,0};

        //The texture uploaded to the GPU
        Texture2D texture;

        TextureGridComponent(GameObject* obj = nullptr) : GameComponent(obj) {
            this->image = GenImageColor(16, 16, BLACK);
            createTexture();
        }

        TextureGridComponent(GameObject* obj, Image image,bool norescale = false, Vector2 centerPixel = {-1,-1}) : GameComponent(obj) {
            this->image = image;
            this->norescale = norescale;
            if(centerPixel.x  == -1 || centerPixel.y == -1){
                this->centerPixel = {floor(static_cast<float>(image.width)/2), floor(static_cast<float>(image.height)/2)};
            }
            else{
                this->centerPixel = centerPixel;
            }
            createTexture();
        }

        void renderUI() override;

        void render() override;

        void update() override;

        void start() override;

        //override destroy
        void onDestroy() override;

        //Updates the texture / uploads it to the GPU to represent the image
        void updateEntireTexture();

        void createTexture();

        void recalculateIslands();
        
        void removeIsland(Image island);

        void seperateIsland(Image island);

        //Converts a pixel position to a world position
        Vector2 pixelToWorld(Vector2 pixelPos);

        //Converts a world position to a pixel position
        Vector2 worldToPixel(Vector2 worldPos);

        bool isValidPixel(Vector2 pixel){
            return pixel.x >= 0 && pixel.x < image.width && pixel.y >= 0 && pixel.y < image.height;
        }

        bool isTransparentPixel(Vector2 pixel){
            return GetImageColor(image, pixel.x, pixel.y).a == 0;
        }

        std::vector<Image> findIslands();

        void floodFill(Vector2 pixel, std::vector<std::vector<bool>>& visited, std::vector<Image>& islands);

        Image cropImageToFit(Image image, Vector2& offset);

        void calcPivot();

        void cropSelfToFit();

        bool delCheck();

        void del() {

        }

};

void disposeIslands(std::vector<Image>& islands);

#endif