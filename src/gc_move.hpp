//gc_move.hpp

#ifndef GC_MOVE_H
#define GC_MOVE_H

#include <stdio.h>
#include "gameObject.hpp"
#include "gameComponent.hpp"

class MoveComponent : public GameComponent {
    public:
        MoveComponent(GameObject* obj = nullptr) : GameComponent(obj) {}

        MoveComponent(GameObject* obj, int speed, bool random) : GameComponent(obj) {
            this->speed = speed;
            this->random = random;
        }

        int speed = 1;

        bool random = false;

        void update() override;
};

#endif