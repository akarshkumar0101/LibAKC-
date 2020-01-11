//
// Created by Akarsh Kumar on 12/11/19.
//

#include "Renderer.h"

static std::vector<float> axisVertices({-100.0,0.0,0.0,
                                         +100.0,0.0,0.0,
                                         0.0,-100.0,0.0,
                                         0.0,+100.0,0.0,
                                         0.0,0.0,-100.0,
                                         0.0,0.0,+100.0,});
static std::vector<float> vertexColors({1.0,0.0,0.0,1.0,
                                        1.0,0.0,0.0,1.0,
                                        0.0,1.0,0.0,1.0,
                                        0.0,1.0,0.0,1.0,
                                        0.0,0.0,1.0,1.0,
                                        0.0,0.0,1.0,1.0,});

static std::vector<unsigned int> axisIndices({2,3, 0,1, 4,5,});




static std::shared_ptr<VertexArray> va(nullptr);
static std::shared_ptr<IndexBuffer> ib(nullptr);

static std::shared_ptr<Shader> axisShader;

void initAxis() {
    axisShader = std::make_shared<Shader>("resources/shaders/axis.shader");

    auto vb = std::make_shared<VertexBuffer>();
    vb->allocateBuffer(6,axisVertices);
    auto vcb = std::make_shared<VertexBuffer>();
    vcb->allocateBuffer(6,vertexColors);

    VertexBufferLayout vbl1;
    vbl1.addElement<float>(3);
    VertexBufferLayout vbl2;
    vbl2.addElement<float>(4);

    va = std::make_shared<VertexArray>();
    va->addBufferWithShaderAttributes(vb, vbl1, *axisShader, {"vPos"});
    va->addBufferWithShaderAttributes(vcb, vbl2, *axisShader, {"axisColor"});

    ib = std::make_shared<IndexBuffer>();
    ib->allocateBuffer(axisIndices);
}

void Renderer::renderAxis(const glm::mat4& projectionView) {
    axisShader->bind();
    axisShader->setUniform("projectionView", projectionView);

    va->bind();
    ib->bind();

    glDrawElements(GL_LINES, ib->count(), GL_UNSIGNED_INT, nullptr);

}

void Renderer::clear(int width, int height){
    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

Renderer::Renderer(bool axis, bool wireFrame): mAxis(axis), mWireFrame(wireFrame) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    if (wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (mAxis) {
        initAxis();
    }
}


void Renderer::renderTriangleWise(const GraphicsData &modelData, const Shader &shader) {
    shader.bind();

    modelData.bind();

    glDrawElements(GL_TRIANGLES, modelData.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::renderLineWise(const GraphicsData &modelData, const Shader &shader) {
    shader.bind();

    modelData.bind();

    glDrawElements(GL_LINES, modelData.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}

//void Renderer::renderRigidBody(const GraphicsData& modelData, const RigidBody &rigidBody, const GraphicsData &jointModel, const Shader &shader) {
//    shader.setUniform("solidColor", glm::vec4(0.0, 0.0, 0.0, 1.0));
//
//    glm::mat4 modelMatrix = rigidBody.transformation();
//    shader.setUniform("model", modelMatrix);
//
//    renderLineWise(modelData, shader);
//
//    for (PointMass pm: rigidBody.pointMasses()) {
//        glm::mat4 pointTransform = pm.transformation();
//        pointTransform = glm::scale(pointTransform, glm::vec3(0.1));
//        shader.setUniform("model", modelMatrix * pointTransform);
//        renderTriangleWise(jointModel, shader);
//    }
//}


//void Renderer::renderForce(const Force &force, const Point &position, const Shader &shader){
//    float forceMag = glm::length(force);
//    glm::mat4 model(1.0);
//
//    model = glm::translate(model,position);
//
//
//    float scaleXY = std::min(1.0f,forceMag);
//    glm::mat4 scaleZ = glm::scale(glm::mat4(1.0),glm::vec3(scaleXY,scaleXY,forceMag));
//    glm::mat4 toZ = glm::rotate(glm::mat4(1.0), (float) M_PI/2, glm::vec3(0.0,1.0,0.0));
//
//    glm::mat4 rotate = glm::lookAt(glm::vec3(0.0),force,perpendicular(force));
//    rotate = glm::inverse(rotate);
//    model = model*rotate*scaleZ*toZ;
//
//    shader.setUniform("model", model);
//
//
//    renderTriangleWise(*CommonModels::arrowModel(), shader);
//}



