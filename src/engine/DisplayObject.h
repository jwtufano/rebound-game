#ifndef DISPLAYOBJECT_H
#define DISPLAYOBJECT_H

#include "AffineTransform.h"
#include "events/EventDispatcher.h"
#include "events/EventListener.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <set>
#include <unordered_set>
#include <string>
#include <vector>

struct jState {
	Sint16 xVal;
	Sint16 yVal;
};

class DisplayObject {

public:
	std::string id = "DEFAULT_ID";
	std::string imgPath = "";
	std::string type = "DisplayObject";

	DisplayObject* parent = NULL;

	int red = 0;
	int green = 0;
	int blue = 0;
	int alpha = 255;
	SDL_Rect sourceRect = {0,0,416,454};

	bool isRGB = false;

	DisplayObject();
	DisplayObject(std::string id, std::string path);
	DisplayObject(std::string id, int red, int green, int blue);
	virtual ~DisplayObject();
	
	virtual void update(std::set<SDL_Scancode> pressedKeys, jState joystickState, std::unordered_set<Uint8> pressedButtons);
	virtual void draw(AffineTransform& at);

	void loadTexture(std::string filepath);
	void loadRGBTexture(int red, int green, int blue);
	void setTexture(SDL_Texture* t);

	void applyTransformations(AffineTransform& at);
	void reverseTransformations(AffineTransform& at);

	void updateSourceRect(SDL_Rect* s);

	int getWidth();
	int getHeight();

	AffineTransform globalTransform;
	AffineTransform getGlobalTransform();

	bool visible = true;
	SDL_Point position = {0, 0};
	int hitbox_width = 100;
	int hitbox_height = 100;
	SDL_Point hitbox_ul = {0,0};
	SDL_Point hitbox_ur = {hitbox_width, 0};
	SDL_Point hitbox_lr = {hitbox_width, hitbox_height};
	SDL_Point hitbox_ll = {0, hitbox_height};
	
	void getHitbox();


	int width = 100;
	int height = 100;
	SDL_Point pivot = {0, 0};
	double scaleX = 1.0;
	double scaleY = 1.0;
	double rotation = 0.0; // in radians
	bool facingRight = true;

	bool hasCollision = false;

private:
	double distance(SDL_Point& p1, SDL_Point& p2);
	double calculateRotation(SDL_Point& origin, SDL_Point& p);
	
	SDL_Texture* texture = NULL;
	SDL_Surface* image = NULL;

	/* Texture currently being drawn. Equal to texture for normal DO */
	SDL_Texture* curTexture;
};

#endif