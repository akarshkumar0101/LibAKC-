//
// Created by Akarsh Kumar on 12/9/19.
//

#include "Node.h"

Node::Node(): mActivationFunction(sigmoid, sigmoidDerivative){
}
float Node::getValue() const {
    return mValue;
}

InputNode::InputNode(unsigned int inputID, InputFeedFunction inputFeedFunction): mInputID(inputID), mInputFeedFunction(inputFeedFunction){
}

void InputNode::compute() {
    mValue = mInputFeedFunction(mInputID);
    this->mValue = mActivationFunction.activationFunction(this->mValue);
}

ConnectedNode::ConnectedNode() {
}

ConnectedNode::ConnectedNode(const std::unordered_set<Node *>& inputNodes) {
    for(Node* inputNode:inputNodes){
        addInputNode(inputNode);
    }
}
void ConnectedNode::addInputNode(const Node *inputNode) {
    this->addInputNode(inputNode, 0.0f);
}

void ConnectedNode::addInputNode(const Node *inputNode, float weightValue) {
    this->mInputNodeWeights[inputNode] = weightValue;
}

void ConnectedNode::compute() {
    this->mValue = 0;
    for(auto it:this->mInputNodeWeights){
        const Node* inputNode = it.first;
        float weight = it.second;
        this->mValue += inputNode->getValue() * weight;
    }
    this->mValue = mActivationFunction.activationFunction(this->mValue);
}


