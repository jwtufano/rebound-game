#pragma once

#include "../../engine/Camera.h"
#include "../../engine/CollisionSystem.h"
#include "../../engine/Game.h"
#include "../../engine/Scene.h"
#include "../../engine/things/Coin.h"

class CollisionDemo : public Game {

public:
    CollisionDemo();
    ~CollisionDemo() override;

private:
    CollisionSystem* collisionSystem;

    void update(unordered_set<SDL_Scancode> pressedKeys, jState joystickState, unordered_set<Uint8> pressedButtons) override;
    void draw(AffineTransform &at) override;
    void draw_post() override;

    Camera* camera;
    Scene* scene;
    Coin* moon;
    Coin* coin;
};
