#ifndef PLAYER_H
#define PLAYER_H

#include "../Sprite.h"
#include "../AnimatedSprite.h"

class Player : public AnimatedSprite {

public:

	Player();

	void changeHealth(int amount);
	void changeStamina(int amount);

	void update(std::unordered_set<SDL_Scancode> pressedKeys, jState joystickState, std::unordered_set<Uint8> pressedButtons) override;
	void draw(AffineTransform& at) override;

private:
	int health = 100;
	int stamina = 1000;

};

#endif
