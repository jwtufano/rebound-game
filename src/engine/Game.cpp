#include "Game.h"

#include "events/ClickEvent.h"
#include "events/DragEvent.h"

#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <chrono>

using namespace std::chrono;

SDL_Renderer* Game::renderer;
Game* Game::instance;
unsigned int Game::frameCounter = 0;

Game::Game(int windowWidth, int windowHeight) {
	Game::instance = this;
	
	this->type = "Game";

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	this->isDragging = false;

	initSDL();
	TTF_Init();
}

Game::~Game() {
	quitSDL();
}

void Game::quitSDL() {
	SDL_DestroyRenderer(Game::renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

void Game::initSDL() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Rebound",
	                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                          this->windowWidth, this->windowHeight,
	                          SDL_WINDOW_ALLOW_HIGHDPI);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	Game::renderer = renderer;
}

void Game::start() {
	milliseconds ms_per_frame(1000ms / this->frames_per_sec);
	auto start = steady_clock::now();

	bool quit = false;
	SDL_Event event;

	while (!quit) {
		auto end = steady_clock::now();
		milliseconds duration = duration_cast<milliseconds>(end - start);
		if (duration > ms_per_frame) {
			start = end;
			this->update(pressedKeys);
			AffineTransform at;
			this->draw(at);
		}

		// While there are events to process:
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE){
					quit = true;
				}
				break;
			case SDL_KEYDOWN:
				pressedKeys.insert(event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				this->pressedKeys.erase(event.key.keysym.scancode);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.windowID == SDL_GetWindowID(window)){
					this->dispatcher.dispatchEvent(new ClickEvent(event.button.x, event.button.y, event.button.button, event.button.clicks));
					this->isDragging = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				this->isDragging = false;
				break;
			case SDL_MOUSEMOTION:
				if (this->isDragging) {
					this->dispatcher.dispatchEvent(new DragEvent(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel));
				}
				break;
			}
		}
	}
}

void Game::update(std::unordered_set<SDL_Scancode> pressedKeys) {
	frameCounter++;
	DisplayObjectContainer::update(pressedKeys);
}

void Game::draw(AffineTransform& at) {
	SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Game::renderer);
	DisplayObjectContainer::draw(at);
	this->draw_post();
	SDL_RenderPresent(Game::renderer);
}
