//gc_move.cpp

#include "gc_move.hpp"
#include <raylib.h>
#include <memory>

void MoveComponent::update() {

    if(IsKeyDown(KEY_LEFT_SHIFT)){
        if(IsKeyDown(KEY_W)){
            gameObject->pivot.y -= speed;
        }
        if(IsKeyDown(KEY_S)){
            gameObject->pivot.y += speed;
        }
        if(IsKeyDown(KEY_A)){
            gameObject->pivot.x -= speed;
        }
        if(IsKeyDown(KEY_D)){
            gameObject->pivot.x += speed;
        }
    }
    else{
        if(IsKeyDown(KEY_W)) {
            gameObject->position.y -= speed;
        }
        if(IsKeyDown(KEY_S)) {
            gameObject->position.y += speed;
        }
        if(IsKeyDown(KEY_A)) {
            gameObject->position.x -= speed;
        }
        if(IsKeyDown(KEY_D)) {
            gameObject->position.x += speed;
        }
    }

    if(IsKeyDown(KEY_Q)) {
        gameObject->rotation -= 1;
    }
    if(IsKeyDown(KEY_E)) {
        gameObject->rotation += 1;
    }

    if(IsKeyPressed(KEY_R)) {
        random = !random;
    }

    if(random){
        if(GetRandomValue(0,5)==1){
            gameObject->position.x += GetRandomValue(-2, 2);
            gameObject->position.y += GetRandomValue(-2, 2);

            gameObject->rotation += GetRandomValue(-1, 1);
        }
    }
}