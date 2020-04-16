#ifndef BASEENEMY_H
#define BASEENEMY_H

#include "../AnimatedSprite.h"
#include "../events/EventListener.h"
#include "../events/EventDispatcher.h"
#include "../things/Player.h"
#include "../Sprite.h"
#include "Projectile.h"

class BaseEnemy : public AnimatedSprite{

public:

    BaseEnemy(std::string id, std::string spritesheet, std::string xml, Player* player);

    void update(std::unordered_set<SDL_Scancode> pressedKeys, jState joystickState, std::unordered_set<Uint8> pressedButtons) override;
    void draw(AffineTransform& at) override;
    //void onCollision(DisplayObject* other);
    //Add onCollision handling when Engine Team gets it together :')


    void changeHealth(int amount);

    //Knowing about the player is nice :)
    Player* player;

    //Info about the Enemies' state
    int health = 100;
    int state = 0;

    //Should we clean this?
    bool clean = false;

};

#endif