//gameScene.cpp

#include "gameScene.hpp"
#include "gameComponent.hpp"
#include "gameObject.hpp"
#include <iostream>

void GameScene::update() {
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->update();
    }
}

void GameScene::render() {
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->render();
    }
}

void GameScene::renderUI() {
    for (int i = 0; i < gameObjects.size(); i++) {
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

