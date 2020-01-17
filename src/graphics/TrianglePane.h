//
// Created by Akarsh Kumar on 1/8/20.
//

#include <vector>

#include "Pane.h"
#include "GraphicsBuffer.h"

class TrianglePane: public Pane {
private:
    GraphicsData triangleData;
    Shader triangleShader;

    static std::vector<float> verticies;
    static std::vector<unsigned int> indices;
public:
    TrianglePane(std::shared_ptr<Window> window): Pane(window), triangleData(verticies, indices), triangleShader("resources/shaders/triangle.shader"){
    }
    ~TrianglePane(){
    }

    void render(const Viewport& viewport) override{
        viewport.bind();
        triangleShader.bind();
        triangleData.bind();
        glDrawElements(GL_TRIANGLES, triangleData.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
    }
};
