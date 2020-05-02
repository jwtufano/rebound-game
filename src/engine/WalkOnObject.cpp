#include "WalkOnObject.h"
#include "./things/Player.h"
#include <iostream>

using namespace std;

WalkOnObject::WalkOnObject(std::string id, SDL_Renderer *r) : EnvironmentObject(id) {
	this->type = "WalkOnObject";
	this->renderer = r;
}
WalkOnObject::WalkOnObject(std::string id, std::string filepath, SDL_Renderer *r) : EnvironmentObject(id, filepath) {
	this->type = "WalkOnObject";
	this->renderer = r;
}
WalkOnObject::WalkOnObject(std::string id, int red, int green, int blue, SDL_Renderer *r) : EnvironmentObject(id, red, green, blue) {
	this->type = "WalkOnObject";
	this->renderer = r;
}
WalkOnObject::WalkOnObject(const DisplayObject& other) : EnvironmentObject(other) {
	this->type = "WalkOnObject";
}

WalkOnObject::~WalkOnObject(){
	
}

void WalkOnObject::update(const std::unordered_set<SDL_Scancode>& pressedKeys, const jState& joystickState, const std::unordered_set<Uint8>& pressedButtons){
	DisplayObject::update(pressedKeys, joystickState, pressedButtons);
}

void WalkOnObject::draw(AffineTransform& at){
	DisplayObject::draw(at);
}

bool WalkOnObject::onCollision(std::shared_ptr<DisplayObject> other){
	if(this->id.find("shield_power_up") != string::npos && other->type == "player"){
		this->visible = false;
		auto player = std::static_pointer_cast<Player>(other);
		player->toggleShieldVisible(true);
		return true;
	}
	else if (this->id.find("mud") != string::npos && other->type == "player"){
		auto player = std::static_pointer_cast<Player>(other);
		player->speed = 0.5;
		player->speedChange = true;
		return true;
	}
	else if (this->id.find("ice") != string::npos && other->type == "player"){
		auto player = std::static_pointer_cast<Player>(other);
		player->speed = 8;
		player->speedChange = true;
		return true;
	}
	else if (this->id.find("spike") != string::npos && other->type == "player"){
			if(collisionWaitTime >= 10){
				collisionWaitTime = 0;
				auto player = std::static_pointer_cast<Player>(other);
				player->changeHealth(-5);
			} else {
				collisionWaitTime++;
			}
			
		return true;
	}
	return false;
}