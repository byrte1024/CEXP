//scene.c

#include "scene.h"
#include "gameObject.h"
#include "ivec2.h"
#include "textureGrid.h"
#include <raylib.h>

Scene* newScene(){
    Scene* scene = (Scene*)malloc(sizeof(Scene));
    scene->count = 0;
    scene->capacity = MAX_OBJECTS;

    scene->objs = (GameObject**)malloc(sizeof(GameObject*) * MAX_OBJECTS);

    for (int i = 0; i < MAX_OBJECTS; i++) {
        scene->objs[i] = NULL;
    }

    return scene;
}
//If all the scene's capacity is used
bool isSceneFull(Scene* scene) {
    if(scene == NULL) return true;
    return scene->count >= MAX_OBJECTS;
}

//If the index in the object array is NULL or a destroyed object
bool isSpotEmpty(Scene* scene, int index) {
    return scene->objs[index] == NULL || scene->objs[index]->destroyed;
}

//Get the first empty spot in the object array
int getEmptySpot(Scene* scene) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (isSpotEmpty(scene, i)) {
            return i;
        }
    }
    return -1;
}

//Get the first replaceable spot in the object array
int getReplaceableSpot(Scene* scene) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (scene->objs[i] != NULL && scene->objs[i]->destroyed) {
            return i;
        }
    }
    return -1;
}

//Get the index of the object in the object array
int getIndex(Scene* scene, GameObject* obj) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (scene->objs[i] == obj) {
            return i;
        }
    }
    return -1;
}

void addObject(Scene* scene, GameObject* obj) {
    if (isSceneFull(scene)) return;
    int index = getEmptySpot(scene);
    if(scene->objs[index] != NULL){
        disposeGameObject(scene->objs[index]);
    }
    scene->objs[index] = obj;
    scene->count++;
}


void removeObject(Scene* scene, GameObject* obj) {
    int index = getIndex(scene, obj);
    if (index == -1) return;
    scene->objs[index] = NULL;
    scene->count--;
}

void removeIndex(Scene* scene, int index) {
    scene->objs[index] = NULL;
    scene->count--;
}

void destroyObject(Scene* scene, GameObject* obj) {
    int index = getIndex(scene, obj);
    if (index == -1) return;
    destroyGameObject(scene->objs[index]);
    scene->count--;
}

void destroyIndex(Scene* scene, int index) {
    if (isSpotEmpty(scene, index)) return;
    destroyGameObject(scene->objs[index]);
    scene->count--;
}

void disposeScene(Scene* scene) {
    free(scene);
}

void disposeSceneAndObjects(Scene* scene) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (scene->objs[i] != NULL) {
            disposeGameObject(scene->objs[i]);
        }
    }
    free(scene);
}

void sceneCallEvent(Scene* scene, EventType eventType) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (isSpotEmpty(scene, i)) continue;
        callEvent(scene->objs[i], eventType);
        
    }
}