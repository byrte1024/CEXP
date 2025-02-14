//gameScene.cpp

#include "gameScene.hpp"
#include "gameComponent.hpp"
#include "gameObject.hpp"
#include <iostream>

void GameScene::update() {
    
    for (int i = gameObjects.size() - 1; i >= 0; i--) {
        if(gameObjects[i]->readytodie){
            gameObjects[i]->destroy();
            gameObjects.erase(gameObjects.begin() + i);
            continue;
        }
        gameObjects[i]->update();
    }
}

void GameScene::render() {
    for (int i = gameObjects.size() - 1; i >= 0; i--) {
        gameObjects[i]->render();
    }
}

void GameScene::renderUI() {
    for (int i = gameObjects.size() - 1; i >= 0; i--) {
        gameObjects[i]->renderUI();
    }
}

std::shared_ptr<GameObject> GameScene::addGameObject(GameObject obj) {
    std::shared_ptr<GameObject> ptr = std::make_shared<GameObject>(obj);
    gameObjects.push_back(ptr);
    return ptr;
}

void GameScene::removeGameObject(std::shared_ptr<GameObject> obj) {
    for (int i = 0; i < gameObjects.size(); i++) {
        if (gameObjects[i] == obj) {
            gameObjects.erase(gameObjects.begin() + i);
            return;
        }
    }
}

