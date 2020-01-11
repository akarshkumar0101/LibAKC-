//
// Created by Akarsh Kumar on 11/28/19.
//

#pragma once

#include "Shader.h"
#include "GraphicsBuffer.h"

void initAxis();

class Renderer{
private:

    bool mAxis;
    bool mWireFrame;

public:
    Renderer(bool axis, bool wireFrame);

    void clear(int width, int height);
    void renderAxis(const glm::mat4& projectionView);

    void renderTriangleWise(const GraphicsData& modelData, const Shader& shader);
    void renderLineWise(const GraphicsData& modelData, const Shader& shader);

//    void renderRigidBody(const GraphicsData& modelData, const RigidBody &rigidBody, const GraphicsData &jointModel, const Shader &shader);

//    void renderForce(const Force& force, const Point& position, const Shader& shader);
};

