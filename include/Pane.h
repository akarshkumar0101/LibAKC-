//
// Created by Akarsh Kumar on 1/3/20.
//

#pragma once

#include <unordered_set>
#include <memory>

#include "Window.h"
#include "glad/glad.h"

class Viewport {
private:
    int mX, mY;
    int mWidth, mHeight;

public:
    Viewport();
    Viewport(const int x, const int y, const int width, const int height);

    int x() const { return mX; }
    int x2()const{ return mX+mWidth; }
    int y()const{ return mY; }
    int y2()const{ return mY+mHeight; }
    int width()const{ return mWidth; }
    int height()const{ return mHeight; }

    void bind()const{glViewport(mX,mY,mWidth,mHeight);};
};

class Window;

class Pane {
private:
    std::weak_ptr<Window> mWindow;
    std::unordered_set<std::shared_ptr<Pane> > mChildren;

protected:
    void addChild(std::shared_ptr<Pane> node);
    void removeChild(std::shared_ptr<Pane> node);

public:
    Pane(std::shared_ptr<Window> window);
    virtual ~Pane();
    virtual void render(const Viewport& viewport) = 0;
};
