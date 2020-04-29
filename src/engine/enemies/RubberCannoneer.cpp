#include "RubberCannoneer.h"

RubberCannoneer::RubberCannoneer(std::shared_ptr<Player> player) : BaseEnemy("RubberCannoneer", "./resources/assets/Animated_Sprites/Enemies/enemies.png", "./resources/assets/Animated_Sprites/Enemies/enemies.xml", "RCannoneerIdle", player){
    this->saveType = "rubber_cannoneer";
}

void RubberCannoneer::update(const std::unordered_set<SDL_Scancode>& pressedKeys, const jState& joystickState, const std::unordered_set<Uint8>& pressedButtons){
    if(this->health <= 0) {
        this->removeThis();
        return;
    }

    if(this->state == 0){
        //init stuff here
    }
    else if(this->state == 1){
        //Waiting lol
        wait = 12;
    }
    else if(this->state == 2){
        if(wait == 12){
            cannonball = std::make_shared<RubberCannonBall>(45);
            cannonball->position = {45,0}; //IDK something so it doesn't spawn on top of the cannon lol
        }
        if(wait == 0){
            this->state = 3;
        }
        wait--;
    }
    else if(this->state == 3){
        this->addChild(cannonball);
        this->cannonball->fire(0);
        wait= 45;
        this->state = 4;
    }
    else if(this->state == 4){
        if(wait == 0){
            this->state = 2;
        }
        else{
            wait--;
        }
    }
    BaseEnemy::update(pressedKeys, joystickState, pressedButtons);
}
