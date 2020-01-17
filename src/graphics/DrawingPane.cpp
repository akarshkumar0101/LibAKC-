//
// Created by Akarsh Kumar on 1/4/20.
//

#include <glm/gtc/matrix_transform.hpp>
#include "DrawingPane.h"
#include "GraphicsBuffer.h"

#include "glm/mat4x4.hpp"



static auto drawingShader(){
    static auto drawingShader = std::make_shared<Shader>("resources/shaders/drawingNodeShader.shader");
    return drawingShader;
}

void Graphics::clear(){
    static bool first = true;
    static auto vb = std::make_shared<VertexBuffer>();
    static auto va = std::make_shared<VertexArray>();
    static auto ib = std::make_shared<IndexBuffer>();
    if(first){
        first = false;
        vb->allocateBuffer(4, std::vector<float>({-1,-1,-1,1,1,-1,1,1}));
        va->addBufferWithShaderAttributes(vb, *VertexBufferLayout::coordinate2DOnlyLayout(), *drawingShader(), {"vPos"});

        ib->allocateBuffer(std::vector<unsigned int>({0,1,2,1,2,3}));
    }

    drawingShader()->bind();
    drawingShader()->setUniform("transform", glm::mat4(1.0));
    va->bind();
    ib->bind();

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);
}

void Graphics::drawPoint(float x, float y) {
    static bool first = true;
    static auto vb = std::make_shared<VertexBuffer>();
    static auto va = std::make_shared<VertexArray>();
    if(first){
        first = false;
        vb->GraphicsBuffer::allocateBuffer(1,1*2*sizeof(float),nullptr,BufferUsage::DYNAMIC);
        va->addBufferWithShaderAttributes(vb, *VertexBufferLayout::coordinate2DOnlyLayout(), *drawingShader(), {"vPos"});
    }
    float data[] = {x,y};
    vb->GraphicsBuffer::changeBufferSubData(0,sizeof(data), data);

    drawingShader()->bind();
    drawingShader()->setUniform("transform", glm::mat4(1.0));
    va->bind();
    glDrawArrays(GL_POINTS,0,1);
}

void Graphics::drawLine(float x1, float y1, float x2, float y2) {
    static bool first = true;
    static auto vb = std::make_shared<VertexBuffer>();
    static auto va = std::make_shared<VertexArray>();
    if(first){
        first = false;
        vb->GraphicsBuffer::allocateBuffer(2,2*2*sizeof(float),nullptr,BufferUsage::DYNAMIC);
        va->addBufferWithShaderAttributes(vb, *VertexBufferLayout::coordinate2DOnlyLayout(), *drawingShader(), {"vPos"});
    }
    float data[] = {x1,y1,x2,y2};
    vb->GraphicsBuffer::changeBufferSubData(0,sizeof(data), data);

    drawingShader()->bind();
    drawingShader()->setUniform("transform", glm::mat4(1.0));
    va->bind();
    glDrawArrays(GL_LINES,0,2);
}

void Graphics::fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    static bool first = true;
    static auto vb = std::make_shared<VertexBuffer>();
    static auto va = std::make_shared<VertexArray>();
    if(first){
        first = false;
        vb->GraphicsBuffer::allocateBuffer(3,3*2*sizeof(float),nullptr,BufferUsage::DYNAMIC);
        va->addBufferWithShaderAttributes(vb, *VertexBufferLayout::coordinate2DOnlyLayout(), *drawingShader(), {"vPos"});
    }
    float data[] = {x1,y1,x2,y2,x3, y3};
    vb->GraphicsBuffer::changeBufferSubData(0,sizeof(data), data);

    drawingShader()->bind();
    drawingShader()->setUniform("transform", glm::mat4(1.0));
    va->bind();
    glDrawArrays(GL_TRIANGLES,0,3);
}
static auto computeCircleVertices(int increments){
    auto vector = std::make_shared<std::vector<float>>();
    vector->insert(vector->end(), {0,0});
    for(int i=0;i <increments;i++){
        float deg = i*2.0f*M_PI / (increments-1);
        vector->push_back(cos(deg));
        vector->push_back(sin(deg));
    }
    return vector;
}

#define CIRCLE_NUM_PARTITIONS 12

void Graphics::fillCircle(float cx, float cy, float r){
    static bool first = true;
    static auto vb = std::make_shared<VertexBuffer>();
    static auto va = std::make_shared<VertexArray>();
    if(first){
        first = false;
        vb->allocateBuffer(CIRCLE_NUM_PARTITIONS+1, *computeCircleVertices(CIRCLE_NUM_PARTITIONS));
        va->addBufferWithShaderAttributes(vb, *VertexBufferLayout::coordinate2DOnlyLayout(), *drawingShader(), {"vPos"});
    }
    glm::mat4 transform(1.0);
    transform = glm::translate(transform, glm::vec3(cx,cy,0.0));
    transform = glm::scale(transform, glm::vec3(r,r,1.0));


    drawingShader()->bind();
    drawingShader()->setUniform("transform", transform);
    va->bind();
    glDrawArrays(GL_TRIANGLE_FAN,0,CIRCLE_NUM_PARTITIONS+1);
}
void Graphics::setColor(const Color& color) {
    drawingShader()->setUniform("color", glm::vec4(color.r, color.g, color.b,color.a));
}

DrawingPane::DrawingPane(std::shared_ptr<Window> window, DrawingFunction drawingFunction): Pane(window), mDrawingFunction(drawingFunction) {
}

void DrawingPane::render(const Viewport &viewport) {
    viewport.bind();
    mDrawingFunction(mGraphics);
}


Color::Color(float r, float g, float b, float a):r(r),g(g),b(b),a(a) {
}
Color Color::RED(1.0,0.0,0.0,1.0);
Color Color::GREEN(0.0,1.0,0.0,1.0);
Color Color::BLUE(0.0,0.0,1.0,1.0);
Color Color::WHITE(1.0,1.0,1.0,1.0);
Color Color::BLACK(0.0,0.0,0.0,1.0);
Color Color::GRAY(0.5,0.5,0.5,1.0);
