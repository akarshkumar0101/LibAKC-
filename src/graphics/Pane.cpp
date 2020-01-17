//
// Created by Akarsh Kumar on 1/3/20.
//

#include "glad/glad.h"
#include "Pane.h"


Viewport::Viewport(): Viewport(0,0,0,0) {
}
Viewport::Viewport(const int x, const int y, const int width, const int height): mX(x), mY(y),mWidth(width), mHeight(height) {
}
void Viewport::bind() const {
    glViewport(mX,mY,mWidth,mHeight);
}

Pane::Pane(std::shared_ptr<Window>window): mWindow(window){
}

void Pane::addChild(std::shared_ptr<Pane> node) {
    mChildren.insert(node);
}
void Pane::removeChild(std::shared_ptr<Pane> node) {
    mChildren.erase(node);
}

Pane::~Pane() {
}




