#pragma once

#include "../engine/Game.h"
#include "../engine/Camera.h"
#include "../engine/Scene.h"
#include "../engine/events/EventListener.h"
#include "../engine/events/MouseDownEvent.h"
#include "../engine/events/MouseUpEvent.h"
#include "../engine/things/Crosshair.h"

#include <vector>
#include <iterator>
#include <algorithm>
#include <filesystem>

using namespace std;

class Editor : public Game, public EventListener {
public:
	Editor();
	Editor(const string& sceneToLoad);

	void setupfiles(const string& path);

	void update(unordered_set<SDL_Scancode> pressedKeys) override;
	void draw(AffineTransform& at) override;
	void initSDL();
	void draw_post() override;

	void cut(unordered_set<DisplayObject*> objects);
	void copy(unordered_set<DisplayObject*> objects);
	void paste();

	void handleEvent(Event* e) override;

	bool onMouseDown(DisplayObject* object, MouseDownEvent* event);
	bool onMouseUp(DisplayObject* object, MouseUpEvent* event);

	SDL_Window* assets_window;
	SDL_Renderer* assets_renderer;

	SDL_Window* edit_window;
	SDL_Renderer* edit_renderer;

private:
	Camera* camera;
	bool inZone = false;
	bool hasChild = false;
	bool grabbedObj = false;
	int obj_ind = 0;
	vector<string> all_sprites;
	Crosshair* crosshair;
	Scene* curScene;

	vector<Sprite*> sprites;
	vector<DisplayObjectContainer*> docs;
	vector<AnimatedSprite*> aSprites;

	DisplayObjectContainer* assets;
	DisplayObjectContainer* assets_sprites;
	DisplayObjectContainer* assets_docs;
	DisplayObjectContainer* assets_aSprites;
	DisplayObjectContainer* edit;

	AffineTransform atTest;

	SDL_Point heldPosition = {0, 0};
	SDL_Point heldPivot = {0, 0};
	double heldScaleX = 1.0;
	double heldScaleY = 1.0;
	double heldRotation = 0.0;

	unordered_set<DisplayObject*> selected;
	unordered_set<DisplayObject*> copied;

	unordered_set<SDL_Scancode> prevKeys;
};
