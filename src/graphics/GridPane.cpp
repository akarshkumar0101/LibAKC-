//
// Created by Akarsh Kumar on 1/3/20.
//

#include <glm/gtc/matrix_transform.hpp>
#include "GridPane.h"


GridPane::GridPane(std::shared_ptr<Window> window, std::pair<unsigned int, unsigned int> gridDimensions): Pane(window), mGridDimensions(std::move(gridDimensions)) {
}


Viewport GridPane::viewportForChild(std::pair<unsigned int, unsigned int> gridLocation, const Viewport& viewport) {
    int width = viewport.width() / mGridDimensions.first;
    int height = viewport.height() / mGridDimensions.second;
    int x = viewport.x() + (((float) gridLocation.first / mGridDimensions.first) * viewport.width());
    int y = viewport.y() + (((float) gridLocation.second / mGridDimensions.second) * viewport.height());

    return Viewport(x, y, width, height);
}

void GridPane::setChild(std::shared_ptr<Pane> node, std::pair<unsigned int, unsigned int> gridLocation){
    Pane::addChild(node);

    auto it = mGridChildren.find(gridLocation);
    if(it != mGridChildren.end()) { // if child already exists
        Pane::removeChild(it->second);
    }
    mGridChildren[gridLocation] = node;
    //viewportForChild(gridLocation);
}
void GridPane::removeChild(std::pair<unsigned int, unsigned int> gridLocation) {
    Pane::removeChild(mGridChildren[gridLocation]);
    mGridChildren.erase(gridLocation);
}





static GraphicsData& horizontalLine(){
    static bool first = true;
    static std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>();
    static std::shared_ptr<VertexArray> va = std::make_shared<VertexArray>();
    static VertexBufferLayout vbl;
    static std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>();

    if(first){
        std::vector<float> verts({0,0,1,0});
        std::vector<unsigned int> inds({0,1});
        vb->allocateBuffer(2,verts);
        ib->allocateBuffer(inds);

        vbl.addElement<float>(2);
        va->addBufferWithDefaultLayout(vb, vbl);
    }

    static GraphicsData horizontalLine(va,ib);

    first = false;
    return horizontalLine;
}
static GraphicsData& verticalLine(){
    static GraphicsData verticalLine({0,0,0,1},{0,1});
    return verticalLine;
}
static Shader& gridShader(){
    static Shader shader("resources/shaders/gridborders.shader");
    return shader;
}
static GraphicsData& tempBuffer(){
    static GraphicsData data({-1,-1,0, -1,1,0, 1,-1,0, 1,1,0},{0,1,0,2,1,3,2,3});
    return data;
}
static GraphicsData& tempBufferX(){
    static GraphicsData data({-1,0,0, 1,0,0},{0,1});
    return data;
}
static GraphicsData& tempBufferY(){
    static GraphicsData data({0,-1,0, 0,1,0},{0,1});
    return data;
}

void GridPane::render(const Viewport& viewport) {
    glm::mat4 iden(1.0);
    for(auto it: mGridChildren){
        Viewport childViewport = viewportForChild(it.first,viewport);

        it.second->render(childViewport);

        childViewport.bind();


        gridShader().bind();
        gridShader().setUniform("solidColor", glm::vec4(1.0,0.0,0.0,1.0));
        glm::mat4 transformp = glm::scale(iden,glm::vec3(0.95f));
        gridShader().setUniform("transform", transformp);

        tempBuffer().bind();
        glDrawElements(GL_LINES, tempBuffer().indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
    }

    viewport.bind();
    gridShader().bind();


    gridShader().setUniform("solidColor", glm::vec4(0.0,1.0,0.0,1.0));
    for(int x=1;x<mGridDimensions.first;x++){
        float xVal = ((float)x/mGridDimensions.first)*2.0f-1.0f;
        glm::mat4 trans = glm::translate(iden,glm::vec3(xVal,0,0));
        gridShader().bind();
        gridShader().setUniform("transform", trans);
        tempBufferY().bind();
        glDrawElements(GL_LINES, tempBufferY().indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
    }
    for(int y=1;y<mGridDimensions.second;y++){
        float yVal = ((float)y / mGridDimensions.second) * 2.0f - 1.0f;
        glm::mat4 trans = glm::translate(iden,glm::vec3(0, yVal, 0));
        gridShader().bind();
        gridShader().setUniform("transform", trans);
        tempBufferX().bind();
        glDrawElements(GL_LINES, tempBufferX().indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
    }
}






