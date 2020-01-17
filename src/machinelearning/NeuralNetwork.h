//
// Created by Akarsh Kumar on 1/12/20.
//

#pragma once

#include <vector>
#include <memory>

#include "Layer.h"


template<class I, class O>
class NeuralNetwork {
private:
    std::vector<std::shared_ptr<LayerRaw>> layers;

    std::shared_ptr<O> previousOutput;

public:
    CostRegime<O> costRegime;

    void addLayer(std::shared_ptr<LayerRaw> layer){
        layers.push_back(layer);
    }

    template<class T, class ... Args>
    void emplaceLayer(Args... args){
        layers.push_back(std::make_shared<T>(args...));
    }

    void compileModel();

    void setCostRegime(CostRegime<O> costRegime){
        this->costRegime = costRegime;
    }

    std::shared_ptr<O> compute(std::shared_ptr<I> input){
        std::shared_ptr<void> output = input;
        for(auto layer: layers){
            output = layer->compute(output);
        }
        previousOutput = std::static_pointer_cast<O>(output);
        return std::static_pointer_cast<O>(output);
    }

    void backprop(O& expectedOutput){
        std::shared_ptr<void> grad = costRegime.costFunctionDerivative(*previousOutput, expectedOutput);
        for(auto it = layers.rbegin();it!=layers.rend();it++){
            grad = (*it)->backprop(grad);
        }

    }
    void takeStep(){
        for(auto it = layers.rbegin();it!=layers.rend();it++){
            (*it)->takeStep();
        }
    }
};