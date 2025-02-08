//gameObject.h

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <raylib.h>
#include <stdlib.h>
#include "ivec2.h"


typedef enum EventType {
    Start,
    Update,
    LateUpdate,
    BeforeRender,
    Render,
    AfterRender,
    Destroy,
    LateDestroy
} EventType;

#define MAX_EVENT_FUNCTIONS 8
#define MAX_DATA 8


typedef struct GameObject {

    IVec2* position;
    float rotation;
    IVec2* scale;

    //Destroyed objects do not trigger events, and are ready to be replaced or removed
    bool destroyed;

    //Some pointer values to be used for custom data
    /*
    IMPORTANT!!! each data field is reserved for:
        - 0: Free
        - 1: Free
        - 2: Free
        - 3: Free
        - 4: Free
        - 5: Free
        - 6: Free
        - 7: Texture Grid
        - 8: Free
    */
    void* data[MAX_DATA];

    //Set up events
    /*
    Event types: 
        - Start (Called when the object is created)

        - Update (Called every frame)

        - Late Update (Called after all updates)

        - Before Render (Called before rendering)
        - Render (Called during rendering, NOTE: THE PROGRAM HAS NO DEFAULT RENDERING FUNCTION, you must implement it yourself)
        - After Render (Called after everything has been rendered)

        - Destroy (Called just before the object is removed from the scene)
        - Late Destroy (Called after the object has been removed from the scene, use this for disposal of custom stuff if needed)
    Each event type should be able to hold up to 8 functions
    */

    void (*Start[MAX_EVENT_FUNCTIONS])(struct GameObject* self);
    void (*Update[MAX_EVENT_FUNCTIONS])(struct GameObject* self);
    void (*LateUpdate[MAX_EVENT_FUNCTIONS])(struct GameObject* self);
    void (*BeforeRender[MAX_EVENT_FUNCTIONS])(struct GameObject* self);
    void (*Render[MAX_EVENT_FUNCTIONS])(struct GameObject* self);
    void (*AfterRender[MAX_EVENT_FUNCTIONS])(struct GameObject* self);
    void (*Destroy[MAX_EVENT_FUNCTIONS])(struct GameObject* self);
    void (*LateDestroy[MAX_EVENT_FUNCTIONS])(struct GameObject* self);

    int StartCount;
    int UpdateCount;
    int LateUpdateCount;
    int BeforeRenderCount;
    int RenderCount;
    int AfterRenderCount;
    int DestroyCount;
    int LateDestroyCount;



} GameObject;

GameObject* newGameObject(int x, int y, float rotation, int scalex, int scaley);

void disposeGameObject(GameObject* obj);

void addEvent(GameObject* obj, void (*event)(GameObject* self), EventType eventType);

void removeEvent(GameObject* obj, void (*event)(GameObject* self), EventType eventType);

void callEvent(GameObject* obj, EventType eventType);

int assignData(GameObject* obj, void* data);

void setData(GameObject* obj, int index, void* data);

void* getData(GameObject* obj, int index);

void freeData(GameObject* obj, int index);

void freeAllData(GameObject* obj);



#endif