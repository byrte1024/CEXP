//gc_render.cpp

#include "gc_render.hpp"
#include "gameObject.hpp"
#include "gameComponent.hpp"
#include <raylib.h>

#include "utils.hpp"

void RenderComponent::render() {
    Rectangle rect = gameObject->getRect();

    DrawRectanglePro(rect, gameObject->pivot * gameObject->size, gameObject->rotation, color);
}