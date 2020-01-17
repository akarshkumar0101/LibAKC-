//
// Created by Akarsh Kumar on 1/13/20.
//

#pragma once
#include <vector>


using InputFeedFunction = float (*)(unsigned int);



using float2float = float (*)(float input);

static float sigmoid(float inp){
    return 1/(1+exp(-inp));
}
static float sigmoidDerivative(float inp){
    float sig = sigmoid(inp);
    return sig*(1-sig);
}

template<class O>
using CostFunction = float (*)(const O& output, const O& expectedOutput);
template<class O>
using CostFunctionDerivative = std::shared_ptr<O> (*)(const O& output, const O& expectedOutput);

static float squareError(const std::vector<float>& output, const std::vector<float>& expectedOutput){
    float error = 0;
    for(int i=0;i<output.size();i++){
        error += (output[i]-expectedOutput[i])*(output[i]-expectedOutput[i]);
    }
    return error;
}
static std::shared_ptr<std::vector<float>> squareErrorGradient(const std::vector<float>& output, const std::vector<float>& expectedOutput){
    auto nodePartials = std::make_shared<std::vector<float>>(output);
    for(int i=0;i<output.size();i++){
        (*nodePartials)[i] = 2*(output[i]-expectedOutput[i]);
    }
    return nodePartials;
}


struct ActivationFunction {
public:
    float2float activationFunction;
    float2float activationFunctionDerivative;

    ActivationFunction(float2float activationFunction, float2float activationFunctionDerivative):
            activationFunction(activationFunction),
            activationFunctionDerivative(activationFunctionDerivative) {
    }
};

static ActivationFunction sigmoidActivationFunction(sigmoid, sigmoidDerivative);



template<class O>
struct CostRegime{
public:
    CostFunction<O> costFunction;
    CostFunctionDerivative<O> costFunctionDerivative;
};

static CostRegime<std::vector<float>> squareErrorRegime({squareError, squareErrorGradient});
