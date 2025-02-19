//gameObject.cpp

#include "gameObject.hpp"
#include "gameComponent.hpp"
#include <stdexcept>
#include <raylib.h>
#include <cmath>
#include <tuple>
#include <array>
#include "utils.hpp"

GameObject::GameObject(std::string name, Vector2 position, Vector2 size, Vector2 pivot, float rotation) {
    this->name = name;
    this->position = position;
    this->size = size;
    this->pivot = pivot;
    this->rotation = rotation;
}

GameObject::GameObject(std::string name, Vector2 position, Vector2 size, Vector2 pivot) {
    this->name = name;
    this->position = position;
    this->size = size;
    this->pivot = pivot;
    this->rotation = 0;
}

GameObject::GameObject(std::string name, Vector2 position, Vector2 size, float rotation) {
    this->name = name;
    this->position = position;
    this->size = size;
    this->pivot = {0, 0};
    this->rotation = rotation;
}

GameObject::GameObject(std::string name, Vector2 position, Vector2 size) {
    this->name = name;
    this->position = position;
    this->size = size;
    this->pivot = {0, 0};
    this->rotation = 0;
}

GameObject::GameObject(std::string name, Vector2 position, float rotation) {
    this->name = name;
    this->position = position;
    this->size = {1, 1};
    this->pivot = {0, 0};
    this->rotation = rotation;
}

GameObject::GameObject(std::string name, Vector2 position) {
    this->name = name;
    this->position = position;
    this->size = {1, 1};
    this->pivot = {0, 0};
    this->rotation = 0;
}

GameObject::GameObject(std::string name) {
    this->name = name;
    this->position = {0, 0};
    this->size = {1, 1};
    this->pivot = {0, 0};
    this->rotation = 0;
}


GameObject::~GameObject() {
    //Destructor
}

void GameObject::setName(std::string name) {
    this->name = name;
}

std::string GameObject::getName() {
    return this->name;
}

Rectangle GameObject::getRect() {
    return Rectangle({position.x, position.y, size.x, size.y});
}

Rectangle GameObject::getRectP() {
    //Since rect uses the x,y as the top left corner, we need to adjust ours with our pivot
    return Rectangle({position.x - (pivot.x), position.y - (pivot.y), size.x, size.y});
}

void GameObject::update() {
    //Loop through all components
    for (int i = 0; i < components.size(); i++) {
        //Call the update function
        components[i]->update();
    }
}

void GameObject::render() {
    //Loop through all components
    for (int i = 0; i < components.size(); i++) {
        //Call the render function
        components[i]->render();
    }

}

void GameObject::destroy() {
    //Loop through all components
    for (int i = 0; i < components.size(); i++) {
        //Call the onDestroy function
        components[i]->onDestroy();
    }
}

void GameObject::renderUI() {
    //Loop through all components
    for (int i = 0; i < components.size(); i++) {
        //Call the renderUI function
        components[i]->renderUI();
    }

    
    if(debugMode==true) {

        std::array<Vector2, 4> corners = getRectCorners();

        //Draw a circle on each corner
        for (int i = 0; i < 4; i++) {
            DrawCircle(corners[i].x, corners[i].y, 4, RED);
        }
        //Form a line between each corner
        for (int i = 0; i < 4; i++) {
            DrawLine(corners[i].x, corners[i].y, corners[(i + 1) % 4].x, corners[(i + 1) % 4].y, RED);
        }

        
        DrawCircle(position.x, position.y, 4, BLUE);
    }
}

Vector2 GameObject::rotateAroundPoint(const Vector2& point, const Vector2& center, float cosTheta, float sinTheta) {
    // Translate point to origin, rotate, then translate back
    float x = point.x - center.x;
    float y = point.y - center.y;
    return {x * cosTheta - y * sinTheta + center.x, x * sinTheta + y * cosTheta + center.y};
}

std::array<Vector2, 4> GameObject::getRectCorners() {
    // Get the rectangle
    const Rectangle& rect = getRectP();
    
    // Compute sine and cosine once
    float rad = rotation * DEG2RAD;
    float cosTheta = get_cosine_by_radians(rad);
    float sinTheta = get_sine_by_radians(rad);

    // Get corners
    Vector2 tl = {rect.x, rect.y};
    Vector2 tr = {rect.x + rect.width, rect.y};
    Vector2 bl = {rect.x, rect.y + rect.height};
    Vector2 br = {rect.x + rect.width, rect.y + rect.height};

    // Rotate corners
    return {
        rotateAroundPoint(tl, position, cosTheta, sinTheta),
        rotateAroundPoint(tr, position, cosTheta, sinTheta),
        rotateAroundPoint(br, position, cosTheta, sinTheta),
        rotateAroundPoint(bl, position, cosTheta, sinTheta)
    };
}

//Destroy


//Start comps
void GameObject::startComps() {
    //Loop through all components
    for (int i = 0; i < components.size(); i++) {
        //Call the start function
        components[i]->start();
    }
}