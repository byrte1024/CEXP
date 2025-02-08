//IVec2

#include "ivec2.h"
#include <raylib.h>
#include <stdio.h>

void disposeIVec2(IVec2* vec) {
    free(vec);
}

void setIVec2(IVec2* self, int x, int y) {
    self->x = x;
    self->y = y;
    self->Vector2Cache = (Vector2){(float)x, (float)y};
}

void addIVec2(IVec2* self, int x, int y) {
    self->x += x;
    self->y += y;
    self->Vector2Cache = (Vector2){(float)self->x, (float)self->y};
}

void subIVec2(IVec2* self, int x, int y) {
    self->x -= x;
    self->y -= y;
    self->Vector2Cache = (Vector2){(float)self->x, (float)self->y};
}

void mulIVec2(IVec2* self, int x, int y) {
    self->x *= x;
    self->y *= y;
    self->Vector2Cache = (Vector2){(float)self->x, (float)self->y};
}

void divIVec2(IVec2* self, int x, int y) {
    self->x /= x;
    self->y /= y;
    self->Vector2Cache = (Vector2){(float)self->x, (float)self->y};
}

void scaleIVec2(IVec2* self, int scale) {
    self->x *= scale;
    self->y *= scale;
    self->Vector2Cache = (Vector2){(float)self->x, (float)self->y};
}


IVec2* newIVec2(int x, int y) {
    IVec2* vec = (IVec2*)malloc(sizeof(IVec2));
    vec->x = x;
    vec->y = y;
    vec->Vector2Cache = (Vector2){(float)x, (float)y};

    vec->set = &setIVec2;
    vec->add = &addIVec2;
    vec->sub = &subIVec2;
    vec->mul = &mulIVec2;
    vec->div = &divIVec2;
    vec->scale = &scaleIVec2;

    return vec;
}

