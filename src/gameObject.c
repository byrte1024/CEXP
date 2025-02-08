
//gameObject.c

#include "gameObject.h"

GameObject* newGameObject(int x, int y, float rotation, int scalex, int scaley) {
    GameObject* obj = (GameObject*)malloc(sizeof(GameObject));
    obj->position = newIVec2(x, y);
    obj->rotation = rotation;
    obj->scale = newIVec2(scalex, scaley);
    obj->destroyed = false;
    obj->StartCount = 0;
    obj->UpdateCount = 0;
    obj->LateUpdateCount = 0;
    obj->BeforeRenderCount = 0;
    obj->RenderCount = 0;
    obj->AfterRenderCount = 0;
    obj->DestroyCount = 0;
    obj->LateDestroyCount = 0;
    
    return obj;
}

void disposeGameObject(GameObject* obj) {
    callEvent(obj, Destroy);
    disposeIVec2(obj->position);
    disposeIVec2(obj->scale);
    free(obj);
    
}

void addEvent(GameObject* obj, void (*event)(GameObject* self), EventType eventType) {
    if(obj->destroyed) return;
    switch(eventType) {
        case Start:
            if (obj->StartCount < MAX_EVENT_FUNCTIONS)
                obj->Start[obj->StartCount++] = event;
            break;
        case Update:
            if (obj->UpdateCount < MAX_EVENT_FUNCTIONS)
                obj->Update[obj->UpdateCount++] = event;
            break;
        case LateUpdate:
            if (obj->LateUpdateCount < MAX_EVENT_FUNCTIONS)
                obj->LateUpdate[obj->LateUpdateCount++] = event;
            break;
        case BeforeRender:
            if (obj->BeforeRenderCount < MAX_EVENT_FUNCTIONS)
                obj->BeforeRender[obj->BeforeRenderCount++] = event;
            break;
        case Render:
            if (obj->RenderCount < MAX_EVENT_FUNCTIONS)
                obj->Render[obj->RenderCount++] = event;
            break;
        case AfterRender:
            if (obj->AfterRenderCount < MAX_EVENT_FUNCTIONS)
                obj->AfterRender[obj->AfterRenderCount++] = event;
            break;
        case Destroy:
            if (obj->DestroyCount < MAX_EVENT_FUNCTIONS)
                obj->Destroy[obj->DestroyCount++] = event;
            break;
        case LateDestroy:
            if (obj->LateDestroyCount < MAX_EVENT_FUNCTIONS)
                obj->LateDestroy[obj->LateDestroyCount++] = event;
            break;
    }
}

void removeEvent(GameObject* obj, void (*event)(GameObject* self), EventType eventType) {
    if(obj->destroyed) return;
    void (**eventArray)(GameObject*) = NULL;
    int* eventCount = NULL;

    switch(eventType) {
        case Start:
            eventArray = obj->Start;
            eventCount = &obj->StartCount;
            break;
        case Update:
            eventArray = obj->Update;
            eventCount = &obj->UpdateCount;
            break;
        case LateUpdate:
            eventArray = obj->LateUpdate;
            eventCount = &obj->LateUpdateCount;
            break;
        case BeforeRender:
            eventArray = obj->BeforeRender;
            eventCount = &obj->BeforeRenderCount;
            break;
        case Render:
            eventArray = obj->Render;
            eventCount = &obj->RenderCount;
            break;
        case AfterRender:
            eventArray = obj->AfterRender;
            eventCount = &obj->AfterRenderCount;
            break;
        case Destroy:
            eventArray = obj->Destroy;
            eventCount = &obj->DestroyCount;
            break;
        case LateDestroy:
            eventArray = obj->LateDestroy;
            eventCount = &obj->LateDestroyCount;
            break;
    }
    
    if (eventArray && eventCount) {
        for (int i = 0; i < *eventCount; i++) {
            if (eventArray[i] == event) {
                for (int j = i; j < *eventCount - 1; j++) {
                    eventArray[j] = eventArray[j + 1];
                }
                (*eventCount)--;
                return;
            }
        }
    }
}

void callEvent(GameObject* obj, EventType eventType) {
    if(obj->destroyed) return;
    void (**eventArray)(GameObject*) = NULL;
    int eventCount = 0;

    switch(eventType) {
        case Start:
            eventArray = obj->Start;
            eventCount = obj->StartCount;
            break;
        case Update:
            eventArray = obj->Update;
            eventCount = obj->UpdateCount;
            break;
        case LateUpdate:
            eventArray = obj->LateUpdate;
            eventCount = obj->LateUpdateCount;
            break;
        case BeforeRender:
            eventArray = obj->BeforeRender;
            eventCount = obj->BeforeRenderCount;
            break;
        case Render:
            eventArray = obj->Render;
            eventCount = obj->RenderCount;
            break;
        case AfterRender:
            eventArray = obj->AfterRender;
            eventCount = obj->AfterRenderCount;
            break;
        case Destroy:
            eventArray = obj->Destroy;
            eventCount = obj->DestroyCount;
            break;
        case LateDestroy:
            eventArray = obj->LateDestroy;
            eventCount = obj->LateDestroyCount;
            break;
    }
    
    for (int i = 0; i < eventCount; i++) {
        eventArray[i](obj);
    }
}

int assignData(GameObject* obj, void* data) {
    if(obj->destroyed) return -1;
    for (int i = 0; i < MAX_DATA; i++) {
        if (obj->data[i] == NULL) {
            obj->data[i] = data;
            return i;
        }
    }
    return -1;
}

void setData(GameObject* obj, int index, void* data) {
    if(obj->destroyed) return;
    obj->data[index] = data;
}

void* getData(GameObject* obj, int index) {
    if(obj->destroyed) return NULL;
    return obj->data[index];
}

void freeData(GameObject* obj, int index) {
    if(obj->destroyed) return;
    if(obj->data[index] == NULL) return;
    free(obj->data[index]);
    obj->data[index] = NULL;
}

void freeAllData(GameObject* obj) {
    if(obj->destroyed) return;
    for (int i = 0; i < MAX_DATA; i++) {
        free(obj->data[i]);
        obj->data[i] = NULL;
    }
}

void destroyGameObject(GameObject* obj) {
    if(obj->destroyed) return;
    callEvent(obj, Destroy);
    obj->destroyed = true;
    callEvent(obj, LateDestroy);
    clearObject(obj);
}

void clearObject(GameObject* obj) {
    freeAllData(obj);
    for (int i = 0; i < MAX_EVENT_FUNCTIONS; i++) {
        obj->Start[i] = NULL;
        obj->Update[i] = NULL;
        obj->LateUpdate[i] = NULL;
        obj->BeforeRender[i] = NULL;
        obj->Render[i] = NULL;
        obj->AfterRender[i] = NULL;
        obj->Destroy[i] = NULL;
        obj->LateDestroy[i] = NULL;
    }
    obj->StartCount = 0;
    obj->UpdateCount = 0;
    obj->LateUpdateCount = 0;
    obj->BeforeRenderCount = 0;
    obj->RenderCount = 0;
    obj->AfterRenderCount = 0;
    obj->DestroyCount = 0;
    obj->LateDestroyCount = 0;
}