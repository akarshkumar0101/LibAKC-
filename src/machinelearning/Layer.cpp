//
// Created by Akarsh Kumar on 1/10/20.
//

#include "Layer.h"

#include <iostream>


//template<class I, class O>
//std::shared_ptr<void> Layer<I, O>::compute(std::shared_ptr<void> input) {
//    std::shared_ptr<I> castedInput = std::static_pointer_cast<I>(input);
//    // does not recurse infinitely, calls the other virtual compute function
//    return compute(castedInput);
//}
//template<class I, class O>
//std::shared_ptr<void> Layer<I,O>::backprop(std::shared_ptr<void> nodePartials){
//    std::shared_ptr<O> casted = std::static_pointer_cast<O>(nodePartials);
//    // does not recurse infinitely, calls the other virtual compute function
//    return backprop(casted);
//}


DenseLayer::DenseLayer(int numNodesPrev, int numNodes, ActivationFunction activationFunction):
    mNumNodesPrev(numNodesPrev), mNumNodes(numNodes),
    mActivationFunction(activationFunction),
    mWeights(mNumNodes, std::vector<float>(mNumNodesPrev, 0.0f)),
    mBiases(mNumNodes),
    mBiasPartials(mNumNodes, 0.0f),
    mWeightPartials(mNumNodes, std::vector<float>(mNumNodesPrev,0.0f)),
    mPreviousZ(mNumNodes, 0.0f) {

    for(int j=0;j<mNumNodes;j++) {
        mBiases[j] = (float) (2*rand()-RAND_MAX)/RAND_MAX;
        for (int k = 0; k < mNumNodesPrev; k++) {
            mWeights[j][k] = (float) (2*rand()-RAND_MAX)/RAND_MAX;
        }
    }
}

std::shared_ptr<std::vector<float>> DenseLayer::compute(std::shared_ptr<std::vector<float>> input) {
    mPreviousInput = input;
    std::shared_ptr<std::vector<float>> output = std::make_shared<std::vector<float>>(mNumNodes);

    if(input->size() != mNumNodesPrev){
        throw std::runtime_error("not valid input for dense layer");
    }

    for(int j=0;j<mNumNodes;j++){
        float val = mBiases[j];
        for(int k = 0; k < mNumNodesPrev; k ++){
            val+= (*input)[k] * mWeights[j][k];
        }
        val = mActivationFunction.activationFunction(val);
        (*output)[j] = val;
    }

    return output;
}


std::shared_ptr<std::vector<float>> DenseLayer::backprop(std::shared_ptr<std::vector<float>> nodePartials) {
    for(int j=0;j<mNumNodes;j++){
        mBiasPartials[j] += (*nodePartials)[j] * mActivationFunction.activationFunctionDerivative(mPreviousZ[j]) * 1.0f;

        for(int k=0;k<mNumNodesPrev;k++){
            mWeightPartials[j][k] += (*nodePartials)[j] * mActivationFunction.activationFunctionDerivative(mPreviousZ[j]) * (*mPreviousInput)[k];
        }
    }
    auto nodePrevPartials = std::make_shared<std::vector<float>>(mNumNodesPrev, 0.0f);
    for(int k=0;k<mNumNodesPrev;k++){
        double sum = 0;
        for(int j=0;j<mNumNodes;j++){
            sum += (*nodePartials)[j] * mActivationFunction.activationFunctionDerivative(mPreviousZ[j]) * mWeights[j][k];
        }
        (*nodePrevPartials)[k] = sum;
    }
    return nodePrevPartials;
}

void DenseLayer::takeStep() {
    for(int j=0; j<mNumNodes;j++){
        mBiases[j] -= mBiasPartials[j]*0.05;
        mBiasPartials[j] = 0.0f; // reset
        for(int k=0;k<mNumNodesPrev;k++){
            mWeights[j][k] -= mWeightPartials[j][k]*0.05;
            mWeightPartials[j][k] = 0.0f; // reset
        }
    }

}
