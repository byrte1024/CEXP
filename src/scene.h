//scene.h

#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gameObject.h"
#include "ivec2.h"
#include "textureGrid.h"
#include <raylib.h>

#define MAX_OBJECTS 32

typedef struct Scene {
    GameObject** objs;
    int count;
    int capacity;

} Scene;

Scene* newScene();

void disposeScene(Scene* scene);

void disposeSceneAndObjects(Scene* scene);

void addObject(Scene* scene, GameObject* obj);

void removeObject(Scene* scene, GameObject* obj);

void removeIndex(Scene* scene, int index);

void destroyObject(Scene* scene, GameObject* obj);

void destroyIndex(Scene* scene, int index);

//If all the scene's capacity is used
bool isSceneFull(Scene* scene);

//If the index in the object array is NULL or a destroyed object
bool isSpotEmpty(Scene* scene, int index);

//Get the first empty spot in the object array
int getEmptySpot(Scene* scene);

int getReplaceableSpot(Scene* scene);

//Get the index of the object in the object array
int getIndex(Scene* scene, GameObject* obj);

//Trigger an event throughout all active objects
void sceneCallEvent(Scene* scene, EventType eventType);

#endif