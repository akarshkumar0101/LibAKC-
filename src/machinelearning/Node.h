//
// Created by Akarsh Kumar on 12/9/19.
//

#pragma once


//#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "Util.h"

class Node{
protected:
    float mValue;
    ActivationFunction mActivationFunction;

public:
    Node();

    virtual void compute() = 0;

    float getValue() const;
};

class InputNode: public Node{
private:
    unsigned int mInputID;
    InputFeedFunction mInputFeedFunction;

public:
    InputNode(unsigned int inputID, InputFeedFunction inputFeedFunction);
    void compute();
};

class ConnectedNode: public Node{
private:
    std::unordered_map<const Node*, float> mInputNodeWeights;

public:
    ConnectedNode();
    ConnectedNode(const std::unordered_set<Node *>& inputNodes);

    void addInputNode(const Node *inputNode);
    void addInputNode(const Node *inputNode, float weightValue);

    void compute();
};

