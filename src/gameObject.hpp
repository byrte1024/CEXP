//gameObject.hpp

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdio.h>
#include <memory>
#include <string>
#include <vector>
#include <raylib.h>
#include "gameComponent.hpp"

using namespace std;

//Assume the existance of a class called GameScene
class GameScene;

//Game objects should store a name, position, and a list of components
//We should store it as GameComponent and not * or & 


class GameObject {
    public:
        std::string name;
        /*
        int x;
        int y;
        int w;
        int h;
        int px;
        int py;
        int rotation
        */
        //Position in world units
        Vector2 position;
        //Size in world units
        Vector2 size;
        //Pivot in world units
        Vector2 pivot;
        float rotation;
        std::vector<std::shared_ptr<GameComponent>> components;

        GameScene* parent;
        /*
        GameObject(std::string name, int x, int y, int w, int h, int px, int py, int rotation);
        GameObject(std::string name, int x, int y, int w, int h, float px, float py, int rotation);
        GameObject(std::string name, int x, int y, int w, int h, int rotation);
        GameObject(std::string name, int x, int y, int w, int h);
        GameObject(std::string name, int x, int y);
        */
        GameObject(std::string name, Vector2 position, Vector2 size, Vector2 pivot, float rotation);
        GameObject(std::string name, Vector2 position, Vector2 size, Vector2 pivot);
        GameObject(std::string name, Vector2 position, Vector2 size, float rotation);
        GameObject(std::string name, Vector2 position, Vector2 size);
        GameObject(std::string name, Vector2 position, float rotation);
        GameObject(std::string name, Vector2 position);
        GameObject(std::string name);
        ~GameObject();

        void setName(std::string name);
        std::string getName();

        /*
        void setX(int x);
        int getX();

        void setY(int y);
        int getY();

        void setW(int w);
        int getW();

        void setH(int h);
        int getH();

        void setPX(int px);
        int getPX();

        void setPY(int py);
        int getPY();

        void setRotation(int rotation);
        int getRotation();

        



        std::tuple<int, int, int, int, int, int, int, int> getRectCorners();

        std::tuple<int,int> rotateAroundPoint(int x, int y, int cx, int cy, int angle);

        */

        Rectangle getRect();

        Rectangle getRectP();

        Vector2 rotateAroundPoint(const Vector2& point, const Vector2& center, float cosTheta, float sinTheta);

        std::array<Vector2, 4> getRectCorners();

        //Add a component to the game object by TYPE
        template <class T>
        T& addComponent() {
            std::shared_ptr<T> component = std::make_shared<T>(this);
            //component->start();
            component->gameObject = this;
            components.push_back(component);

            return *component;
        }
        //Add a component to the game object by VALUE (creates a copy)
        template <class T>
        T& addComponent(T component) {
            std::shared_ptr<T> newComponent = std::make_shared<T>(component);
            newComponent->gameObject = this;
            newComponent->start();
            //newComponent->start();
            components.push_back(newComponent);

            return *newComponent;
        }
        //Get a component from the game object by TYPE
        template <class T>
        T& getComponent() {
            for (int i = 0; i < components.size(); i++) {
                if (dynamic_cast<T*>(components[i].get())) {
                    return *dynamic_cast<T*>(components[i].get());
                }
            }
        }

        //Remove a component from the game object by TYPE
        template <class T>
        void removeComponent() {
            for (int i = 0; i < components.size(); i++) {
                if (dynamic_cast<T*>(components[i].get())) {
                    components[i]->end();
                    components.erase(components.begin() + i);
                    return;
                }
            }
        }

        //Update all components
        void update();

        //Render all components
        void render();

        //Render all UI components
        void renderUI();

        
};

#endif