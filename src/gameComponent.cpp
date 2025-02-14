//gameComponent.cpp

#include "gameComponent.hpp"
#include "gameObject.hpp"
#include <raylib.h>



//function definitions

GameComponent::~GameComponent() {
    //Destructor
}

void GameComponent::start() {
    //Called when the component is added to a game object (Note, can be before loop starts and can be outside of scene)
}

void GameComponent::update() {
    //Called every frame
}

void GameComponent::render() {
    //Called during virtual rendering (on the projection texture), use for objects

}

void GameComponent::renderUI() {
    //Called during real rendering (on the screen texture), use for UI
}

void GameComponent::end() {
    //Called when the component is removed from a game object
}

void GameComponent::onDestroy() {
    //Called when the attached game object is removed (and just before the component is removed)
}
