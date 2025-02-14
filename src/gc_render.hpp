//gc_render

#ifndef GC_RENDER_H
#define GC_RENDER_H

#include <stdio.h>
#include <raylib.h>
#include "gameObject.hpp"
#include "gameComponent.hpp"

class RenderComponent : public GameComponent {
    public:
        RenderComponent(GameObject* obj = nullptr) : GameComponent(obj) {}

        RenderComponent(GameObject* obj, Color color) : GameComponent(obj) {
            this->color = color;
        }

        Color color = RED;

        void render() override;
};

#endif