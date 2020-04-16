#include "Layer.h"
#include "Game.h"
#include <iostream>

Layer::Layer() : DisplayObject("layer") {
    this->type = "layer";
}

Layer::Layer(std::string id) : DisplayObject(id) {
    this->type = "layer";
}

void Layer::setParallax(){
    position.x = -cam->pivot.x * parallaxSpeed;
}

void Layer::draw(AffineTransform& at) {
    Layer::setParallax();
    Layer::draw(at, Game::renderer);
}

void Layer::draw(AffineTransform& at, SDL_Renderer* r, SDL_Rect* src) {
    Layer::setParallax();

    applyTransformations(at);
    // undo the parent's pivot
    at.translate(pivot.x, pivot.y);
    for (auto child : children) {
        child->draw(at);
    }
    // redo the parent's pivot
    at.translate(-pivot.x, -pivot.y);
    reverseTransformations(at);
}