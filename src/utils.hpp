//utils.hpp

#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>
#include <cmath>
#include "gameScene.hpp"

#define TABLE_SIZE 512

// Lookup table for sine (and cosine)
inline double sine_table[TABLE_SIZE];

inline void generate_lookup_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        double angle = (2 * PI * i) / TABLE_SIZE;
        sine_table[i] = sin(angle);
    }
}

inline void print_lookup_table() {
    printf("Sine Table:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("%d: %f\n", i, sine_table[i]);
    }
}

inline double get_sine(int index) {
    return sine_table[index % TABLE_SIZE];
}

inline double get_cosine(int index) {
    return sine_table[(index + TABLE_SIZE / 4) % TABLE_SIZE];
}


inline double get_sine_by_radians(double radians) {
    int index = (int)(radians * TABLE_SIZE / (2 * PI)) % TABLE_SIZE;
    if (index < 0) index += TABLE_SIZE;
    return get_sine(index);
}

inline double get_cosine_by_radians(double radians) {
    int index = (int)(radians * TABLE_SIZE / (2 * PI)) % TABLE_SIZE;
    if (index < 0) index += TABLE_SIZE;
    return get_cosine(index);
}

inline double get_sine_by_degrees(double degrees) {
    return get_sine_by_radians(degrees * PI / 180.0);
}

inline double get_cosine_by_degrees(double degrees) {
    return get_cosine_by_radians(degrees * PI / 180.0);
}

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
        static_cast<float>(v.x * get_cosine_by_radians(rads) - v.y * get_sine_by_radians(rads)),
        static_cast<float>(v.x * get_sine_by_radians(rads) + v.y * get_cosine_by_radians(rads))
    };
}



#endif