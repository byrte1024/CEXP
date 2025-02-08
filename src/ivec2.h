//IVec2.h

#ifndef IVEC2_H
#define IVEC2_H

#include <stdlib.h>
#include <raylib.h>


typedef struct IVec2 {
    int x;
    int y;
    Vector2 Vector2Cache;

    void (*set)(struct IVec2* self, int x, int y);
    void (*add)(struct IVec2* self, int x, int y);
    void (*sub)(struct IVec2* self, int x, int y);
    void (*mul)(struct IVec2* self, int x, int y);
    void (*div)(struct IVec2* self, int x, int y);
    void (*scale)(struct IVec2* self, int scale);
} IVec2;

IVec2* newIVec2(int x, int y);

void disposeIVec2(IVec2* vec);

void setIVec2(IVec2* self, int x, int y);

void addIVec2(IVec2* self, int x, int y);

void subIVec2(IVec2* self, int x, int y);

void mulIVec2(IVec2* self, int x, int y);

void divIVec2(IVec2* self, int x, int y);

void scaleIVec2(IVec2* self, int scale);



#endif