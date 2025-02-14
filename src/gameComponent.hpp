//gameComponent.hpp

#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <stdio.h>

inline bool debugMode = true;

// We can't import GameObject (circle dependency)
// So let's just assume it exists
class GameObject;

// A component is a class that can be attached to a game object
// It should have many overridable functions
class GameComponent {
public:
    // Pointer to the game object this component is attached to (can be null)
    GameObject* gameObject;

    // Constructor
    GameComponent(GameObject* obj = nullptr) : gameObject(obj) {}

    // Destructor
    virtual ~GameComponent();

    /// Events ///

    // Called when the component is added to a game object (Note, can be before loop starts and can be outside of scene)
    virtual void start();

    // Called every frame
    virtual void update();

    // Called during virtual rendering (on the projection texture), use for objects
    virtual void render();

    // Called during real rendering (on the screen texture), use for UI
    virtual void renderUI();

    // Called when the component is removed from a game object
    virtual void end();

    // Called when the attached game object is removed (and just before the component is removed)
    virtual void onDestroy();
};

#endif
