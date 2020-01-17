//
// Created by Akarsh Kumar on 12/9/19.
//

#pragma once

#include <unordered_map>
#include <vector>

#include "Node.h"




class LayeredNeuralNetwork {
private:
    std::vector<std::unordered_set<Node*>> mLayers;
    InputFeedFunction mInputFeedFunction;

    std::vector<float> mOutput;
public:
    LayeredNeuralNetwork(const InputFeedFunction& inputFeedFunction);
    LayeredNeuralNetwork(const InputFeedFunction& inputFeedFunction, const std::vector<unsigned int>& nodesInLayers);
    ~LayeredNeuralNetwork();

    void addLayer(unsigned int numNodes);

    void computeFully();
    void computeLayer(unsigned int layer);

    const InputFeedFunction& inputFeedFunction() const{
        return mInputFeedFunction;
    }

    const std::vector<float>& output() const {
        return mOutput;
    }
};



