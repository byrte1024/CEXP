//gameScene.hpp

#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <vector>
#include <memory>
#include <raylib.h>
#include "gameObject.hpp"
#include "gameComponent.hpp"
#include "utils.hpp"

class GameScene {
    public:
        std::vector<std::shared_ptr<GameObject>> gameObjects;

        void update();

        void render();

        void renderUI();

        /*
        //Adds a game object to the scene (by reference)
        void addGameObjectRef(std::shared_ptr<GameObject> obj) {
            gameObjects.push_back(obj);
        }
        */

        //Adds a game object to the scene (by value)
        std::shared_ptr<GameObject> addGameObject(GameObject obj);

        //Removes a game object from the scene
        void removeGameObject(std::shared_ptr<GameObject> obj) ;





};


#endif