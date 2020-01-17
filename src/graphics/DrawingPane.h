//
// Created by Akarsh Kumar on 1/4/20.
//

#include <vector>
#include <memory>
#include "Pane.h"

struct Color{
public:
    float r,g,b,a;
    Color(float r, float g,float b,float a);

    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color WHITE;
    static Color BLACK;
    static Color GRAY;
};

class Graphics {
private:
public:
    void clear();
    void drawPoint(float x, float y);
    void drawLine(float x1, float y1, float x2, float y2);
//    void outlineTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
    void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
    void fillCircle(float cx, float cy, float r);
    void setColor(const Color& color);
};

using DrawingFunction = void (*)(Graphics& graphics);

class DrawingPane: public Pane {
private:
    Graphics mGraphics;
    DrawingFunction mDrawingFunction;

public:
    DrawingPane(std::shared_ptr<Window> window, DrawingFunction drawingFunction);

    void render(const Viewport& viewport) override;
};


