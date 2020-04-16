#include "Editor.h"

#include "../engine/events/DisplayTreeChangeEvent.h"
#include "../engine/events/DragEvent.h"
#include "../engine/events/DragStartEvent.h"

#include <cmath>
#include <iostream>

namespace fs = std::filesystem;

using namespace std;

Editor::Editor()
    : Editor("./resources/scene/blank.json") {}

Editor::Editor(const string& sceneToLoad)
    : Game(1200, 800) {
    this->initSDL();

    this->collisionSystem = new CollisionSystem();
    EventDispatcher::getInstance().addEventListener(this->collisionSystem, DisplayTreeChangeEvent::DISPLAY_TREE_CHANGE_EVENT);
    EventDispatcher::getInstance().addEventListener(this, MouseDownEvent::MOUSE_DOWN_EVENT);
    EventDispatcher::getInstance().addEventListener(this, DragEvent::DRAG_EVENT);
    EventDispatcher::getInstance().addEventListener(this, DragStartEvent::DRAG_START_EVENT);
    EventDispatcher::getInstance().addEventListener(this, MouseUpEvent::MOUSE_UP_EVENT);
    EventDispatcher::getInstance().addEventListener(this, MouseMotionEvent::MOUSE_MOTION_EVENT);
    EventDispatcher::getInstance().addEventListener(this, WindowEnterEvent::WINDOW_ENTER_EVENT);
    EventDispatcher::getInstance().addEventListener(this, WindowExitEvent::WINDOW_EXIT_EVENT);
    EventDispatcher::getInstance().addEventListener(this, TextInputEvent::TEXT_INPUT_EVENT);
    EventDispatcher::getInstance().addEventListener(this, TextEditEvent::TEXT_EDIT_EVENT);
    EventDispatcher::getInstance().addEventListener(this, MouseWheelEvent::MOUSE_WHEEL_EVENT);

    instance = this;
    curScene = new Scene();

    camera = new Camera();
    camera->position = {this->windowWidth / 2, this->windowHeight / 2};
    camera->pivot = {this->windowWidth / 2, this->windowHeight / 2};
    instance->addChild(camera);

    curScene->loadScene(sceneToLoad);

    camera->addChild(curScene);

    assets = new DisplayObject("assets");
    assets->renderer = this->assets_renderer;

    edit = new DisplayObject("edit");
    edit->renderer = this->edit_renderer;

    TextObject* idLabel = new TextObject(string("idLabel"), string("ID:"), Game::font, edit_renderer);

    TextObject* posLabel  = new TextObject(string("posLabel"),  string("Position"), Game::font, edit_renderer);
    TextObject* posXLabel = new TextObject(string("posXLabel"), string("X:"),       Game::font, edit_renderer);
    TextObject* posYLabel = new TextObject(string("posYLabel"), string("Y:"),       Game::font, edit_renderer);
    posLabel->position = {0, 40};
    posXLabel->position = {0, 70};
    posYLabel->position = {150, 70};

    TextObject* pivLabel  = new TextObject(string("pivLabel"),  string("Pivot"), Game::font, edit_renderer);
    TextObject* pivXLabel = new TextObject(string("pivXLabel"), string("X:"),    Game::font, edit_renderer);
    TextObject* pivYLabel = new TextObject(string("pivYLabel"), string("Y:"),    Game::font, edit_renderer);
    pivLabel->position = {0, 110};
    pivXLabel->position = {0, 140};
    pivYLabel->position = {150, 140};

    TextObject* scaleLabel  = new TextObject(string("scaleLabel"),  string("Scale"), Game::font, edit_renderer);
    TextObject* scaleXLabel = new TextObject(string("scaleXLabel"), string("X:"),    Game::font, edit_renderer);
    TextObject* scaleYLabel = new TextObject(string("scaleYLabel"), string("Y:"),    Game::font, edit_renderer);
    scaleLabel->position = {0, 180};
    scaleXLabel->position = {0, 210};
    scaleYLabel->position = {150, 210};

    TextObject* rotLabel = new TextObject(string("rotLabel"), string("Rotation:"), Game::font, edit_renderer);
    rotLabel->position = {0, 250};

    DisplayObject* layerZeroBackground = new DisplayObject("layerZeroBackground", 36, 113, 96, 37, 25, edit_renderer);
    layerZeroBackground->position = {0, 425};

    TextObject* layerZeroButton = new TextObject(string("layerZeroButton"), string("L0"), Game::font, edit_renderer);
    layerZeroButton->position = {5, 420};

    DisplayObject* layerOneBackground = new DisplayObject("layerOneBackground", 36, 113, 96, 37, 25, edit_renderer);
    layerOneBackground->position = {40, 425};

    TextObject* layerOneButton = new TextObject(string("layerOneButton"), string("L1"), Game::font, edit_renderer);
    layerOneButton->position = {45, 420};

    DisplayObject* layerTwoBackground = new DisplayObject("layerTwoBackground", 36, 113, 96, 37, 25, edit_renderer);
    layerTwoBackground->position = {80, 425};

    TextObject* layerTwoButton = new TextObject(string("layerTwoButton"), string("L2"), Game::font, edit_renderer);
    layerTwoButton->position = {85, 420};

    DisplayObject* layerZeroIndicator = new DisplayObject("layerZeroIndicator", 0, 255, 0, 10, 10, edit_renderer);
    layerZeroIndicator->position = {15, 410};

    DisplayObject* layerOneIndicator = new DisplayObject("layerOneIndicator", 0, 255, 0, 10, 10, edit_renderer);
    layerOneIndicator->position = {55, 410};
    layerOneIndicator->visible = false;

    DisplayObject* layerTwoIndicator = new DisplayObject("layerTwoIndicator", 0, 255, 0, 10, 10, edit_renderer);
    layerTwoIndicator->position = {95, 410};
    layerTwoIndicator->visible = false;
    
    edit->addChild(idLabel);
    edit->addChild(posLabel);
    edit->addChild(posXLabel);
    edit->addChild(posYLabel);
    edit->addChild(pivLabel);
    edit->addChild(pivXLabel);
    edit->addChild(pivYLabel);
    edit->addChild(scaleLabel);
    edit->addChild(scaleXLabel);
    edit->addChild(scaleYLabel);
    edit->addChild(rotLabel);
    edit->addChild(layerZeroBackground);
    edit->addChild(layerZeroButton);
    edit->addChild(layerOneBackground);
    edit->addChild(layerOneButton);
    edit->addChild(layerTwoBackground);
    edit->addChild(layerTwoButton);
    edit->addChild(layerZeroIndicator);
    edit->addChild(layerOneIndicator);
    edit->addChild(layerTwoIndicator);

    setupfiles("./resources/assets");
}

void Editor::setupfiles(const string& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path() == "./resources/assets/Spritesheets") {
            for (const auto& AS : fs::directory_iterator(entry.path())) {
                //AnimatedSprite* temp = new AnimatedSprite(AS.path().stem().string(), AS.path().string() + "/" + AS.path().stem().string() + ".png", AS.path().string() + "/" + AS.path().stem().string() + ".xml", assets_renderer);
                //temp->play("Idle");
                aSprites.push_back(new AnimatedSprite(AS.path().stem().string(), AS.path().string() + "/" + AS.path().stem().string() + ".png", AS.path().string() + "/" + AS.path().stem().string() + ".xml", assets_renderer));
            }
        } else if (entry.path() == "./resources/assets/Display_Objects") {
            for (const auto& DO : fs::directory_iterator(entry.path())) {
                dos.push_back(new DisplayObject(DO.path().stem().string(), DO.path().string(), assets_renderer));
            }
        } else if (entry.path() == "./resources/assets/Sprites") {
            for (const auto& S : fs::directory_iterator(entry.path())) {
                sprites.push_back(new Sprite(S.path().stem().string(), S.path().string(), assets_renderer));
            }
        }
    }
    for (int i = 0; i < aSprites.size(); ++i) {
        aSprites[i]->position.x = i % 2 == 0 ? 0 : 150;
        aSprites[i]->position.y = (i / 2) * 150;
        aSprites[i]->height = 150;
        aSprites[i]->width = 150;
        assets->addChild(aSprites[i]);
    }
    for (int i = 0; i < dos.size(); ++i) {
        dos[i]->position.x = (i + aSprites.size()) % 2 == 0 ? 0 : 150;
        dos[i]->position.y = ((i + aSprites.size()) / 2) * 150;
        dos[i]->height = 150;
        dos[i]->width = 150;
        assets->addChild(dos[i]);
    }
    for (int i = 0; i < sprites.size(); ++i) {
        sprites[i]->position.x = (i + aSprites.size() + dos.size()) % 2 == 0 ? 0 : 150;
        sprites[i]->position.y = ((i + aSprites.size() + dos.size()) / 2) * 150;
        sprites[i]->height = 150;
        sprites[i]->width = 150;
        assets->addChild(sprites[i]);
    }
}

void Editor::update(std::unordered_set<SDL_Scancode> pressedKeys, jState joystickState, std::unordered_set<Uint8> pressedButtons) {
    // Check if we are in text input mode
    if (SDL_IsTextInputActive() == SDL_TRUE){
        // If we are in text input mode, these commands are active

        if (pressedKeys.find(SDL_SCANCODE_BACKSPACE) != pressedKeys.end() &&
        prevKeys.find(SDL_SCANCODE_BACKSPACE) == prevKeys.end()){
            attributeSelected->text.pop_back();
            attributeSelected->setText(attributeSelected->text);
            
            this->updateAttribute();
        }
    } else{
        // If we are not in text input mode, these normal controls are active

        // Move camera around scene
        if (pressedKeys.find(SDL_SCANCODE_RIGHT) != pressedKeys.end()) {
            int f = (int)5 * (1 / camera->scaleX);
            if (f < 1) {
                f = 1;
            }
            camera->panRight(f);
        }
        if (pressedKeys.find(SDL_SCANCODE_LEFT) != pressedKeys.end()) {
            int f = (int)5 * (1 / camera->scaleX);
            if (f < 1) {
                f = 1;
            }
            camera->panLeft(f);
        }
        if (pressedKeys.find(SDL_SCANCODE_DOWN) != pressedKeys.end()) {
            int f = (int)5 * (1 / camera->scaleY);
            if (f < 1) {
                f = 1;
            }
            camera->panDown(f);
        }
        if (pressedKeys.find(SDL_SCANCODE_UP) != pressedKeys.end()) {
            int f = (int)5 * (1 / camera->scaleY);
            if (f < 1) {
                f = 1;
            }
            camera->panUp(f);
        }

        // Zoom camera in and out
        if (!(SDL_GetModState() & KMOD_CTRL) && pressedKeys.find(SDL_SCANCODE_X) != pressedKeys.end()) {
            camera->zoomIn(1.1);
        }
        if (!(SDL_GetModState() & KMOD_CTRL) && pressedKeys.find(SDL_SCANCODE_Z) != pressedKeys.end()) {
            camera->zoomOut(1.1);
        }

        // Deselect objects and assets
        if (((SDL_GetModState() & KMOD_CTRL) && pressedKeys.find(SDL_SCANCODE_D) != pressedKeys.end()) ||
            pressedKeys.find(SDL_SCANCODE_ESCAPE) != pressedKeys.end()) {
            this->selected.clear();
            if (selectedAsset){
                curScene->getChild(layer)->removeImmediateChild(selectedAsset);
                selectedAsset = NULL;
            }
            editSelected = NULL;
        }

        // Cut, copy, and paste objects
        if (SDL_GetModState() & KMOD_CTRL && pressedKeys.find(SDL_SCANCODE_X) != pressedKeys.end() &&
            prevKeys.find(SDL_SCANCODE_X) == prevKeys.end()) {
            this->cut(this->selected);
        }

        if (SDL_GetModState() & KMOD_CTRL && pressedKeys.find(SDL_SCANCODE_C) != pressedKeys.end() &&
            prevKeys.find(SDL_SCANCODE_C) == prevKeys.end()) {
            this->copy(this->selected);
        }

        if (SDL_GetModState() & KMOD_CTRL && pressedKeys.find(SDL_SCANCODE_V) != pressedKeys.end() &&
            prevKeys.find(SDL_SCANCODE_V) == prevKeys.end()) {
            this->paste();
        }

        if (pressedKeys.find(SDL_SCANCODE_DELETE) != pressedKeys.end() &&
            prevKeys.find(SDL_SCANCODE_DELETE) == prevKeys.end()) {
            for (DisplayObject* object : this->selected){
                object->removeThis();
            }
            this->selected.clear();
        }

        // Old version of editing object attributes, probably obsolete now

        // // Pivot
        // if (pressedKeys.find(SDL_SCANCODE_I) != pressedKeys.end()) {
        //     heldPivot.y -= 5;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_J) != pressedKeys.end()) {
        //     heldPivot.x -= 5;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_K) != pressedKeys.end()) {
        //     heldPivot.y += 5;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_L) != pressedKeys.end()) {
        //     heldPivot.x += 5;
        // }

        // // Rotation
        // if (pressedKeys.find(SDL_SCANCODE_Q) != pressedKeys.end()) {
        //     heldRotation -= 0.05;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_W) != pressedKeys.end()) {
        //     heldRotation += 0.05;
        // }

        // // Scaling
        // if (pressedKeys.find(SDL_SCANCODE_MINUS) != pressedKeys.end()) {
        //     heldScaleX -= 0.05;
        //     heldScaleY -= 0.05;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_EQUALS) != pressedKeys.end()) {
        //     heldScaleX += 0.05;
        //     heldScaleY += 0.05;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_O) != pressedKeys.end()) {
        //     heldScaleX -= 0.05;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_P) != pressedKeys.end()) {
        //     heldScaleX += 0.05;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_LEFTBRACKET) != pressedKeys.end()) {
        //     heldScaleY -= 0.05;
        // }
        // if (pressedKeys.find(SDL_SCANCODE_RIGHTBRACKET) != pressedKeys.end()) {
        //     heldScaleY += 0.05;
        // }

        // Jason's debugging tool
        if (pressedKeys.find(SDL_SCANCODE_Y) != pressedKeys.end()) {
            if (setParentMode){
                cout << "Set Parent Mode" << endl;
            }
        }

        // Save scene to file
        if ((SDL_GetModState() & KMOD_CTRL) && pressedKeys.find(SDL_SCANCODE_S) != pressedKeys.end()) {
            string tmp;
            getline(cin, tmp);
            curScene->saveScene(tmp);
        }
    }

    edit->getChild("layerZeroIndicator")->visible = layer == 0;
    edit->getChild("layerOneIndicator")->visible = layer == 1;
    edit->getChild("layerTwoIndicator")->visible = layer == 2;
    
    prevKeys = pressedKeys;

    Game::update(pressedKeys, joystickState, pressedButtons);
    assets->update(pressedKeys, joystickState, pressedButtons);
    edit->update(pressedKeys, joystickState, pressedButtons);
}

void Editor::draw(AffineTransform& at) {
    this->clearRenderers();
	DisplayObject::draw(at);

    assets->draw(at);
    edit->draw(at);

	this->draw_post();
	this->presentRenderers();
}

void Editor::initSDL() {
    assets_window = SDL_CreateWindow("Assets", 0, 500, 300, 450, SDL_WINDOW_ALLOW_HIGHDPI);

    edit_window = SDL_CreateWindow("Edit", 0, 0, 300, 450, SDL_WINDOW_ALLOW_HIGHDPI);

    assets_renderer = SDL_CreateRenderer(assets_window, -1, 0);
    edit_renderer = SDL_CreateRenderer(edit_window, -1, 0);
}

void Editor::draw_post() {
    SDL_SetRenderDrawColor(Game::renderer, 90, 90, 90, SDL_ALPHA_OPAQUE);

    // Draw gridlines.
    int startX = this->camera->getGlobalPosition().x % lround(Editor::GRID_SIZE * this->camera->getZoom());
    for (int x = startX; x < this->windowWidth; x += lround(Editor::GRID_SIZE * this->camera->getZoom())) {
        if (x - this->camera->getGlobalPosition().x == 0) {
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderDrawLine(Game::renderer, x, 0, x, this->windowHeight);
        if (x - this->camera->getGlobalPosition().x == 0) {
            SDL_SetRenderDrawColor(Game::renderer, 90, 90, 90, SDL_ALPHA_OPAQUE);
        }
    }

    int startY = this->camera->getGlobalPosition().y % lround(Editor::GRID_SIZE * this->camera->getZoom());
    for (int y = startY; y < this->windowHeight; y += lround(Editor::GRID_SIZE * this->camera->getZoom())) {
        if (y - this->camera->getGlobalPosition().y == 0) {
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderDrawLine(Game::renderer, 0, y, this->windowWidth, y);
        if (y - this->camera->getGlobalPosition().y == 0) {
            SDL_SetRenderDrawColor(Game::renderer, 90, 90, 90, SDL_ALPHA_OPAQUE);
        }
    }

    if (SDL_IsTextInputActive() && attributeSelected) {
        attributeSelected->drawHitbox({255, 255, 255, SDL_ALPHA_OPAQUE});
    }

    for (DisplayObject* object : this->selected) {
        object->drawHitbox({255, 255, 255, SDL_ALPHA_OPAQUE});
    }
    
}

void Editor::clearRenderers(){
    Game::clearRenderers();

    SDL_SetRenderDrawColor(Editor::assets_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Editor::assets_renderer);
    
    SDL_SetRenderDrawColor(Editor::edit_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Editor::edit_renderer);
}

void Editor::presentRenderers(){
    Game::presentRenderers();
    SDL_RenderPresent(Editor::assets_renderer);
    SDL_RenderPresent(Editor::edit_renderer);
}

void Editor::cut(const unordered_set<DisplayObject*>& objects) {
    // this->copied.clear();
    // if (!objects.empty()){
    //     for (DisplayObject* object : objects) {
    //         DisplayObject* copy;
    //         if (object->type == "DisplayObject"){
    //             copy = new DisplayObject(*object);
    //         } else if (object->type == "Sprite"){
    //             copy = new Sprite(*object);
    //         } else if (object->type == "AnimatedSprite"){
    //             copy = new AnimatedSprite(*object);
    //         }
    //         copy->r = Game::renderer;
            
    //         this->copied.insert(copy);
    //         object->removeThis();

    //         this->selected.erase(object);
    //         this->displacementX.erase(object);
    //         this->displacementY.erase(object);
    //     }
    // }

    this->copied.clear();
    for (DisplayObject* object : objects) {
        // Make the copy now so that future changes won't affect the copy
        DisplayObject* copy;
        if (object->type == "DisplayObject"){
            copy = new DisplayObject(*object);
        } else if (object->type == "Sprite"){
            copy = new Sprite(*object);
        } else if (object->type == "AnimatedSprite"){
            copy = new AnimatedSprite(*object);
        }
        copy->renderer = Game::renderer;

        // Offset a bit
        copy->position.x += 16;
        copy->position.y += 16;

        // if (!keepHierarchy){
        //     copy->parent = NULL;
        //     copy->children.clear();
        // }

        this->copied.insert(copy);

        object->removeThis();

        this->displacementX.erase(object);
        this->displacementY.erase(object);
    }
    this->selected.clear();
}

void Editor::copy(const unordered_set<DisplayObject*>& objects, bool keepHierarchy) {
    this->copied.clear();
    for (DisplayObject* object : objects) {
        // Make the copy now so that future changes won't affect the copy
        DisplayObject* copy;
        if (object->type == "DisplayObject"){
            copy = new DisplayObject(*object);
        } else if (object->type == "Sprite"){
            copy = new Sprite(*object);
        } else if (object->type == "AnimatedSprite"){
            copy = new AnimatedSprite(*object);
        }
        copy->renderer = Game::renderer;

        // Offset a bit
        copy->position.x += 16;
        copy->position.y += 16;

        if (!keepHierarchy){
            copy->parent = NULL;
            copy->children.clear();
        }

        this->copied.insert(copy);

    }
    if (selectedAsset){
        selectedAsset = *copied.begin();
        if (selectedAsset->type == "AnimatedSprite"){
            cout << "Playing animation" << endl;
            //((AnimatedSprite*) selectedAsset)->play("Idle");
        }
        curScene->getChild(layer)->addChild(selectedAsset);
    }
}

void Editor::paste(MouseDownEvent* event) {
    if (!event){
        this->selected.clear();
    }
    for (DisplayObject* object : this->copied) {
        // TODO: This always inserts at the root.
        // Should that always be the case?
        this->curScene->getChild(layer)->addChild(object);
        if (!event){
            this->selected.insert(object);
        } // else{
        //     object->position.x = -lround((1/camera->getZoom())*this->windowWidth  / 2);
        //     object->position.y = -lround((1/camera->getZoom())*this->windowHeight / 2);
        //     object->position.x += camera->pivot.x;
        //     object->position.x += lround(event->x*(1/camera->getZoom()));
        //     object->position.y += camera->pivot.y;
        //     object->position.y += lround(event->y*(1/camera->getZoom()));
        // }
    }

    // Pre-emptively get ready to copy the same objects again
    unordered_set<DisplayObject*> temp = this->copied;
    this->copy(temp);
}

void Editor::handleEvent(Event* e) {
    if (e->getType() == MouseDownEvent::MOUSE_DOWN_EVENT) {
        MouseDownEvent* event = static_cast<MouseDownEvent*>(e);
        if (event->wID == SDL_GetWindowID(assets_window)){
            assetsWindowActive = true;
            editWindowActive = mainWindowActive = false;

            DisplayObject* temp = NULL;
            if (!this->selected.empty()){
                temp = *(this->selected.begin());
            }
            this->selected.clear();
            if (selectedAsset){
                curScene->getChild(layer)->removeImmediateChild(selectedAsset);
                selectedAsset = NULL;
            }
            if (this->onMouseDown(assets, event)){
                if (*this->selected.begin() == temp){
                    this->selected.clear();
                } else{
                    this->copy(this->selected, false);
                    selectedAsset = *copied.begin();
                    selectedAsset->visible = false;
                    curScene->getChild(layer)->addChild(selectedAsset);
                }
            }

        } else if (event->wID == SDL_GetWindowID(edit_window)){
            editWindowActive = true;
            assetsWindowActive = mainWindowActive = false;

            this->attributeSelected = NULL;

            if (this->onMouseDown(edit, event)){

            }

        } else{
            mainWindowActive = true;
            assetsWindowActive = editWindowActive = false;
            if (selectedAsset){
                this->paste(event);
                if (selectedAsset){
                    selectedAsset->position.x = -lround((1/camera->getZoom())*this->windowWidth  / 2);
                    selectedAsset->position.y = -lround((1/camera->getZoom())*this->windowHeight / 2);
                    selectedAsset->position.x += camera->pivot.x;
                    selectedAsset->position.x += lround(event->x*(1/camera->getZoom()));
                    selectedAsset->position.y += camera->pivot.y;
                    selectedAsset->position.y += lround(event->y*(1/camera->getZoom()));
                }
            } else if (!this->onMouseDown(this, event)) {
                this->selected.clear();
            }
        }
        
        // Remove the children that show values if selected differs from currently displayed values
        if (selected.size() != 1 || editSelected != *selected.begin()){
            // Store pre-modification number of children
            int tempNumChildren = edit->numChildren();

            //Delete all children beyond the default 20 that are always displayed
            for (int i = 20; i < tempNumChildren; ++i){
                edit->removeImmediateChild(edit->getChild(20));
            }

            // Clear the editable objects
            editable.clear();
        }
        
        // Update edit window if there is one non-asset object selected that differs from currently displayed object
        if (!selectedAsset && selected.size() == 1 && editSelected != *selected.begin()){ 
            editSelected = *selected.begin();

            TextObject* idText = new TextObject(string("idText"), string(editSelected->id) , Game::font, edit_renderer);
            idText->position = {37, 0};

            TextObject* posXText = new TextObject(string("posXText"),  to_string(editSelected->position.x), Game::font, edit_renderer);
            TextObject* posYText = new TextObject(string("posYText"),  to_string(editSelected->position.y), Game::font, edit_renderer);
            posXText->position = {25, 70};
            posYText->position = {175, 70};

            TextObject* pivXText = new TextObject(string("pivXText"),  to_string(editSelected->pivot.x), Game::font, edit_renderer);
            TextObject* pivYText = new TextObject(string("pivYText"),  to_string(editSelected->pivot.y), Game::font, edit_renderer);
            pivXText->position = {25, 140};
            pivYText->position = {175, 140};

            TextObject* scaleXText = new TextObject(string("scaleXText"),  to_string(editSelected->scaleX), Game::font, edit_renderer);
            TextObject* scaleYText = new TextObject(string("scaleYText"),  to_string(editSelected->scaleY), Game::font, edit_renderer);
            scaleXText->position = {25, 210};
            scaleYText->position = {175, 210};

            TextObject* rotText = new TextObject(string("rotText"),  to_string(editSelected->rotation), Game::font, edit_renderer);
            rotText->position = {110, 250};

            DisplayObject* parentButtonBackground = new DisplayObject("parentButtonBackground", 36, 113, 96, 125, 25, edit_renderer);
            parentButtonBackground->position = {0, 295};

            TextObject* setParentButton = new TextObject(string("setParentButton"), string("Set Parent"), Game::font, edit_renderer);
            setParentButton->position = {5, 290};

            
            
            edit->addChild(idText);
            edit->addChild(posXText);
            edit->addChild(posYText);
            edit->addChild(pivXText);
            edit->addChild(pivYText);
            edit->addChild(scaleXText);
            edit->addChild(scaleYText);
            edit->addChild(rotText);
            edit->addChild(parentButtonBackground);
            edit->addChild(setParentButton);

            editable.insert(idText);
            editable.insert(posXText);
            editable.insert(posYText);
            editable.insert(pivXText);
            editable.insert(pivYText);
            editable.insert(scaleXText);
            editable.insert(scaleYText);
            editable.insert(rotText);
        } else if (selected.size() == 1 && editSelected != *selected.begin()){
            editSelected = NULL;
        }
    } else if (e->getType() == DragStartEvent::DRAG_START_EVENT) {
        this->displacementX.clear();
        this->displacementY.clear();

        for (DisplayObject* object : this->selected) {
            this->displacementX.insert({object, 0.0});
            this->displacementY.insert({object, 0.0});
        }
    } else if (e->getType() == DragEvent::DRAG_EVENT) {
        DragEvent* event = static_cast<DragEvent*>(e);
        if (event->wID == SDL_GetWindowID(assets_window)){
            assetsWindowActive = true;
            editWindowActive = mainWindowActive = false;

        } else if (event->wID == SDL_GetWindowID(edit_window)){
            editWindowActive = true;
            assetsWindowActive = mainWindowActive = false;

        } else if (event->wID == SDL_GetWindowID(this->window) && !selectedAsset){
            mainWindowActive = true;
            assetsWindowActive = editWindowActive = false;

            for (DisplayObject* object : this->selected) {
                // Okay, this is convoluted. Here's a walkthrough...

                // Calculate movement threshold
                double threshold = this->camera->getZoom();
                if (!(event->modifiers & KMOD_ALT)) {
                    // Snap by the grid size if we are not holding alt
                    threshold *= Editor::GRID_SIZE;
                }

                // Add the relative movement generated by this drag event to the
                // total displacement that we need to move the object by
                this->displacementX.at(object) += event->xrel;
                this->displacementY.at(object) += event->yrel;

                // Move the object based on the current zoom level (pixel movement != screen movement),
                // being sure to truncate (round towards zero) the amount we move by.
                // Snap by the threshold (but use raw pixels as the AffineTransform will handle zoom).
                // We must truncate and not round in cases such as zoom = 2.0, where rounding causes
                // us to *always* round away from zero, resulting in mouse/object de-sync.
                double pixels = threshold / this->camera->getZoom();
                object->position.x += static_cast<int>(pixels * trunc(this->displacementX.at(object) / threshold));
                object->position.y += static_cast<int>(pixels * trunc(this->displacementY.at(object) / threshold));

                // Now that we've moved the object, reset the displacement we need to move by
                // for the _next_ iteration by calculating the unused remainder.
                // We use fmod() and not remainder() because the return value of remainder() is
                // kept within 1/2 the denominator (in other words, it is (-1/2 * denom, 1/2 * denom]),
                // whereas the result of fmod is [0, denom)
                this->displacementX.at(object) = fmod(this->displacementX.at(object), threshold);
                this->displacementY.at(object) = fmod(this->displacementY.at(object), threshold);
            }
        }
    } else if (e->getType() == MouseUpEvent::MOUSE_UP_EVENT) {
        MouseUpEvent* event = static_cast<MouseUpEvent*>(e);
        if (event->wID == SDL_GetWindowID(edit_window)){
            if (this->onMouseUp(edit, event)){
                if (attributeSelected){
                    SDL_StartTextInput();
                    Hitbox hitbox = attributeSelected->getHitbox();
                    SDL_Rect rect{hitbox.ul.x, hitbox.ul.y, hitbox.ur.x - hitbox.ul.x, hitbox.ur.y - hitbox.ul.y};
                    SDL_SetTextInputRect(&rect);
                    cout << "Started input" << endl;
                }
            } else{
                if (SDL_IsTextInputActive() == SDL_TRUE){
                    SDL_StopTextInput();
                    cout << "Stopped input" << endl;
                    attributeSelected = NULL;
                }
            }
        } else if (event->wID == SDL_GetWindowID(this->window)){
            this->onMouseUp(this, event);
            if (SDL_IsTextInputActive() == SDL_TRUE){
                SDL_StopTextInput();
                cout << "Stopped input" << endl;
                attributeSelected = NULL;
            }
        } else if (event->wID == SDL_GetWindowID(assets_window)){
            this->onMouseUp(assets, event);
            if (SDL_IsTextInputActive() == SDL_TRUE){
                SDL_StopTextInput();
                cout << "Stopped input" << endl;
                attributeSelected = NULL;
            }
        }
    } else if (e->getType() == MouseMotionEvent::MOUSE_MOTION_EVENT) {
        MouseMotionEvent* event = static_cast<MouseMotionEvent*>(e);
        if (selectedAsset && event->wID == SDL_GetWindowID(this->window)){
            if (selectedAsset && !selectedAsset->visible){
                selectedAsset->visible = true;
            }
            selectedAsset->position.x = -lround((1/camera->getZoom())*this->windowWidth  / 2);
            selectedAsset->position.y = -lround((1/camera->getZoom())*this->windowHeight / 2);
            selectedAsset->position.x += camera->pivot.x;
            selectedAsset->position.x += lround(event->x*(1/camera->getZoom()));
            selectedAsset->position.y += camera->pivot.y;
            selectedAsset->position.y += lround(event->y*(1/camera->getZoom()));
        }
    } else if (e->getType() == WindowEnterEvent::WINDOW_ENTER_EVENT) {
        WindowEnterEvent* event = static_cast<WindowEnterEvent*>(e);
        if (event->wID == SDL_GetWindowID(assets_window)){
            assetsWindowActive = true;
            editWindowActive = mainWindowActive = false;
            
            SDL_SetWindowInputFocus(assets_window);
        } else if (event->wID == SDL_GetWindowID(edit_window)){
            editWindowActive = true;
            assetsWindowActive = mainWindowActive = false;

            SDL_SetWindowInputFocus(edit_window);
        } else if (event->wID == SDL_GetWindowID(this->window)){
            mainWindowActive = true;
            assetsWindowActive = editWindowActive = false;

            SDL_SetWindowInputFocus(this->window);

            if (selectedAsset){
                selectedAsset->visible = true;
            }
        }
        
    } else if (e->getType() == WindowExitEvent::WINDOW_EXIT_EVENT) {
        WindowExitEvent* event = static_cast<WindowExitEvent*>(e);
        if (selectedAsset && event->wID == SDL_GetWindowID(this->window)){
            selectedAsset->visible = false;
        }
        
    } else if (e->getType() == TextInputEvent::TEXT_INPUT_EVENT){
        TextInputEvent* event = static_cast<TextInputEvent*>(e);
        if (attributeSelected){
            if (attributeSelected->id == "idText"){
                attributeSelected->setText(string(attributeSelected->text + event->text));
            } else{
                if (isdigit(event->text[0]) ||
                    (event->text[0] == '.' && attributeSelected->text.find(".") == string::npos && attributeSelected->text != " " && attributeSelected->text != "-") ||
                    (event->text[0] == '-' && attributeSelected->text == " ")){

                    string strNew = "";
                    if (attributeSelected->text != " "){
                        strNew += attributeSelected->text;
                    }
                    attributeSelected->setText(string(strNew + event->text));
                }
            }
            this->updateAttribute();
        }
    } else if (e->getType() == TextEditEvent::TEXT_EDIT_EVENT){
        cout << "Editing is happening" << endl;
        TextEditEvent* event = static_cast<TextEditEvent*>(e);
        if (attributeSelected){
            if (attributeSelected->id == "idText"){
                attributeSelected->setText(event->text);
            } else{
                if (isdigit(event->text[0]) || event->text[0] == '.'){
                    attributeSelected->setText(event->text);
                }
            }
        }
    } else if (e->getType() == MouseWheelEvent::MOUSE_WHEEL_EVENT){
        MouseWheelEvent* event = static_cast<MouseWheelEvent*>(e);
        if (event->wID == SDL_GetWindowID(assets_window)){
            assets->position.y += event->y * 20;
            if (assets->position.y < -((assets->numChildren()+1)/2)*150){
                assets->position.y = 0;
            }
            if (assets->position.y > 450){
                assets->position.y = -((assets->numChildren()+1)/2)*150 + 150;
            }
        }
    }
}

bool Editor::onMouseDown(DisplayObject* object, MouseDownEvent* event) {
    // Reverse iterator to check the topmost objects first
    for (auto it = object->children.crbegin(); it != object->children.crend(); it++) {
        if (this->onMouseDown(*it, event)) {
            return true;
        }
    }

    if (CollisionSystem::isInside({event->x, event->y}, object->getHitbox())) {
        if (setParentMode){
            if (object != editSelected && event->wID == SDL_GetWindowID(window)){
                auto it = std::find(editSelected->parent->children.cbegin(), editSelected->parent->children.cend(), editSelected);
                if (it != editSelected->parent->children.cend()) {
                    editSelected->parent->children.erase(it);
                }
                object->addChild(editSelected);
                // editSelected->parent = object;
                // editSelected->parentId = object->id;
                setParentMode = false;
                edit->removeImmediateChild("setParentModeIndicator");
            }
        } else if (!(event->modifiers & KMOD_CTRL) && event->wID != SDL_GetWindowID(edit_window) || event->wID == SDL_GetWindowID(assets_window)) {
            // If this object is not already selected and we are not ctrl-clicking,
            // unselect everything
            if (this->selected.find(object) == this->selected.cend()) {
                this->selected.clear();
            }

            // Then mark this object as selected
            this->selected.insert(object);
        }
        return true;
    }
    return false;
}

bool Editor::onMouseUp(DisplayObject* object, MouseUpEvent* event) {
    // Reverse iterator to check the topmost objects first
    for (auto it = object->children.crbegin(); it != object->children.crend(); it++) {
        if (this->onMouseUp(*it, event)) {
            return true;
        }
    }

    if (CollisionSystem::isInside({event->x, event->y}, object->getHitbox())) {
        if (event->wID == SDL_GetWindowID(edit_window)){
            if (editable.find(object) != editable.end() && object->type == "TextObject"){
                attributeSelected = (TextObject*) object;
                return true;
            } else if (object->id == "parentButtonBackground" || object->id == "setParentButton"){
                setParentMode = !setParentMode;
                if (setParentMode){
                    DisplayObject* indicator = new DisplayObject("setParentModeIndicator", 255, 0, 0, 10, 10, edit_renderer);
                    indicator->position = {140, 300};
                    edit->addChild(indicator);
                } else{
                    edit->removeImmediateChild("setParentModeIndicator");
                }
                return true;
            } else if (object->id == "layerZeroBackground" || object->id == "layerZeroButton"){
                layer = 0;
            } else if (object->id == "layerOneBackground" || object->id == "layerOneButton"){
                layer = 1;
            } else if (object->id == "layerTwoBackground" || object->id == "layerTwoButton"){
                layer = 2;
            }
            return false;
        } else if (!(event->modifiers & KMOD_CTRL)) {
            // If this object is already selected and we are not ctrl-clicking,
            // clear everything and select ourselves
            if (this->selected.find(object) != this->selected.cend()) {
                this->selected.clear();
                this->selected.insert(object);
            }
        } else {
            if (this->selected.find(object) != this->selected.cend()) {
                // If ctrl-click and we are already selected, unselect only ourselves
                this->selected.erase(object);
            } else {
                // If ctrl-click and we are not already selected, select ourselves
                this->selected.insert(object);
            }
        }
        return true;
    }

    return false;
}

void Editor::updateAttribute(){
    // Fetch the current text of the attribute
    string valToSet = attributeSelected->text;

    // If we are updating the id, set immediately and return
    if (attributeSelected->id == "idText"){
        editSelected->id = valToSet;
        return;
    }

    // If the value is a single space or minus
    // set the value to 0.0001 if we are updating scale, set to 0 otherwise
    if (valToSet == " " || valToSet == "-"){
        if (attributeSelected->id == "scaleXText" || attributeSelected->id == "scaleYText"){
            valToSet = "0.0001";
        } else {
            valToSet = "0";
        }
    }

    // Check what field we are updating, and do so accordingly
    if (attributeSelected->id == "posXText"){
        editSelected->position.x = stoi(valToSet);
    } else if (attributeSelected->id == "posYText"){
        editSelected->position.y = stoi(valToSet);
    } else if (attributeSelected->id == "pivXText"){
        editSelected->pivot.x = stoi(valToSet);
    } else if (attributeSelected->id == "pivYText"){
        editSelected->pivot.y = stoi(valToSet);
    } else if (attributeSelected->id == "scaleXText"){
        double val = stod(valToSet);
        if (val - 0.0001 < 0){
            editSelected->scaleX = 0.0001;
        } else{
            editSelected->scaleX = val;
        }
    } else if (attributeSelected->id == "scaleYText"){
        double val = stod(valToSet);
        if (val - 0.0001 < 0){
            editSelected->scaleY = 0.0001;
        } else{
            editSelected->scaleY = val;
        }
    } else if (attributeSelected->id == "rotText"){
        editSelected->rotation = stod(valToSet);
    }
}