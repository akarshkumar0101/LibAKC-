//
// Created by Akarsh Kumar on 12/9/19.
//

#include "LayeredNeuralNetwork.h"

#define NUM_LAYERS this->mLayers.size()
#define INPUT_LAYER 0
#define OUTPUT_LAYER this->mLayers.size()-1

LayeredNeuralNetwork::LayeredNeuralNetwork(const InputFeedFunction& inputFeedFunction): mInputFeedFunction(inputFeedFunction) {
}
LayeredNeuralNetwork::LayeredNeuralNetwork(const InputFeedFunction& inputFeedFunction, const std::vector<unsigned int>& nodesInLayers): LayeredNeuralNetwork(inputFeedFunction) {
    for(unsigned int nodesInLayer: nodesInLayers){
        addLayer(nodesInLayer);
    }
}

LayeredNeuralNetwork::~LayeredNeuralNetwork() {
    // TODO destruct all nodes
}

void LayeredNeuralNetwork::addLayer(unsigned int numNodes) {
    unsigned int newLayerID = mLayers.size();
    mLayers.emplace_back();
    for (unsigned int nodeID = 0; nodeID < numNodes; nodeID++) {
        Node* node = nullptr;
        if(newLayerID==0){
            InputNode* inode = new InputNode(nodeID, mInputFeedFunction);
            node = inode;
        } else {
            ConnectedNode* cnode = new ConnectedNode(mLayers[newLayerID-1]);
            node = cnode;
        }

        mLayers.back().insert(node);
    }
}

void LayeredNeuralNetwork::computeFully() {
    for(unsigned int layer = 0; layer< NUM_LAYERS;layer++){
        computeLayer(layer);
    }
}

void LayeredNeuralNetwork::computeLayer(unsigned int layer){
    if(layer==OUTPUT_LAYER){
        mOutput.resize(mLayers.at(OUTPUT_LAYER).size());
    }
    int i = 0;
    for(Node* node: mLayers.at(layer)) {
        node->compute();

        if (layer == OUTPUT_LAYER) {
            mOutput.at(i++) = node->getValue();
        }
    }
}



