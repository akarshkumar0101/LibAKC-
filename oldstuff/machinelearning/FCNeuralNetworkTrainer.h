//
//  NeuralNetworkTrainer.h
//  LibAKC++
//
//  Created by Akarsh Kumar on 5/23/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#ifndef NeuralNetworkTrainer_h
#define NeuralNetworkTrainer_h

#include <cmath>
#include <limits>
#include <tuple>

class FCAbstractNeuralNetworkTrainer {
public:
    // return a double such that arr[0] is the input and arr[1] is the
    // expectedOutput
    virtual std::tuple<double*, double*> getRandomTrainingExample(int inputLayerSize, int outputLayerSize)=0;
    
    virtual std::vector<std::tuple<double*, double*>>* getRandomTrainingExamples(int batchSize, int inputLayerSize, int outputLayerSize) {
        std::vector<std::tuple<double*, double*>>* examples = new std::vector<std::tuple<double*, double*>>(batchSize);
        for (int i = 0; i < batchSize; i++) {
            (*examples)[i] = this->getRandomTrainingExample(inputLayerSize, outputLayerSize);
        }
        return examples;
    }
    
    static constexpr double STANDARD_ACCEPTABLE_ERROR = 0.05;
    
    virtual double acceptableOutputError() {
        return STANDARD_ACCEPTABLE_ERROR;
    }
    
    virtual bool isCorrect(double* output, double* expectedOutput, int length) {
        for (int i = 0; i < length; i++) {
            if (abs((output[i] - expectedOutput[i]) / expectedOutput[i]) > this->acceptableOutputError())
                return false;
        }
        return true;
    }
    
    virtual double getCost(double* output, double* expectedOutput, int length) {
        double cost = 0;
        for (int i = 0; i < length; i++) {
            double del = output[i] - expectedOutput[i];
            cost += del*del;
        }
        return cost;
    }
    
    virtual double getDerivativeOfCost(double output, double expectedOutput) {
        return 2 * (output - expectedOutput);
    }
    
    virtual double getWeightLearningRate()=0;
    
    virtual double getBiasLearningRate() {
        return this->getWeightLearningRate();
    }
    
    virtual int getN() {
        return INT_MAX;
    }
    
    virtual void runEveryNIterations(double* beforePerformance, double* afterPerformance) {
    }
    
    
};

//D = Data, A = Answer
template<class D, class A>
class FCNeuralNetworkTrainer: FCAbstractNeuralNetworkTrainer {
    
public:
    virtual void rawToInputLayer(const D& d, double* inputLayer, const int inputLayerSize) =0;
    
    virtual void rawToOutputLayer(const A& a, double* outputLayer, const int outputLayerSize) =0;
    
    /**
     * @return a [? instance of D, ? instance of A];
     */
    virtual std::tuple<D, A> getRandomRawData()=0;
    
    
    // return a double such that arr[0] is the input and arr[1] is the
    // expectedOutput
    virtual std::tuple<double*, double*> getRandomTrainingExample(int inputLayerSize, int outputLayerSize){
        auto [d,a] = this->getRandomRawData();
        double* inparr = new double[inputLayerSize];
        double* outarr = new double[outputLayerSize];
        this->rawToInputLayer(d, inparr, inputLayerSize);
        this->rawToOutputLayer(a, outarr, outputLayerSize);
        return std::make_tuple(inparr, outarr);
    }
    
    
};


#endif /* NeuralNetworkTrainer_h */
