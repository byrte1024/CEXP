//utils.hpp

#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>
#include <cmath>
#include "gameScene.hpp"

inline GameScene* currentScene;

inline std::string version = ""; 

//Multiply a vector by a scalar
inline Vector2 operator*(Vector2 v, float f) {
    return {v.x * f, v.y * f};
}

//Multiply a scalar by a vector
inline Vector2 operator*(float f, Vector2 v) {
    return {v.x * f, v.y * f};
}

//Add two vectors
inline Vector2 operator+(Vector2 v1, Vector2 v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

//Subtract two vectors
inline Vector2 operator-(Vector2 v1, Vector2 v2) {
    return {v1.x - v2.x, v1.y - v2.y};
}

//Divide a vector by a scalar
inline Vector2 operator/(Vector2 v, float f) {
    return {v.x / f, v.y / f};
}

//Divide a scalar by a vector
inline Vector2 operator/(float f, Vector2 v) {
    return {f / v.x, f / v.y};
}

//Multiply a vector by a vector
inline Vector2 operator*(Vector2 v1, Vector2 v2) {
    return {v1.x * v2.x, v1.y * v2.y};
}

//Divide a vector by a vector
inline Vector2 operator/(Vector2 v1, Vector2 v2) {
    return {v1.x / v2.x, v1.y / v2.y};
}

//Rotate a vector by an angle (in degrees)
//Rotate a vector by an angle (in degrees)
inline Vector2 rotateVector(Vector2 v, float angle) {
    float rads = angle * (PI / 180);
    return {
        static_cast<float>(v.x * cos(rads) - v.y * sin(rads)),
        static_cast<float>(v.x * sin(rads) + v.y * cos(rads))
    };
}


#endif