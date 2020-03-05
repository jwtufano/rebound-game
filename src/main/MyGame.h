#ifndef MYGAME_H
#define MYGAME_H

#include "../engine/Game.h"
#include "../engine/Sprite.h"
#include "../engine/AnimatedSprite.h"
#include "../engine/Scene.h"
#include "../engine/Camera.h"
#include "../engine/things/Coin.h"
#include "../engine/things/Player.h"
#include "../engine/QuestManager.h"
#include "../engine/events/PickedUpEvent.h"
#include "../engine/Sound.h"
#include "../tweens/Tween.h"
#include "../tweens/TweenJuggler.h"


class MyGame : public Game {

public:
	MyGame();
	virtual ~MyGame();

	void update(std::set<SDL_Scancode> pressedKeys) override;
	void draw(AffineTransform& at) override;

private:

	Camera* camera;
	bool inZone = false;

	Player* player;
	Coin* coin;
	Coin* coin1;
	Coin* coin2;
	
	QuestManager* questManager;

	// DisplayObject* character;
	Scene* scene;
	Scene* scene2;

	Sound* collect;
	Tween* player_tween;
	Tween* coin_tween1;
	Tween* coin_tween2;
	Tween* coin_tween3;
	TweenJuggler* juggler;
};

#endif