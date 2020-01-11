//
//  NeuralNetwork.h
//  LibAKC++
//
//  Created by Akarsh Kumar on 5/15/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#ifndef NeuralNetwork_h
#define NeuralNetwork_h

#include <math.h>
#include <vector>

#include "FCNeuralNetworkTrainer.h"
#include "TensorMath.h"


template<int NUM_LAYERS>
class FCNeuralNetwork {
#define INPUT_LAYER 0
#define OUTPUT_LAYER NUM_LAYERS-1
    //MatrixD<3,3> connections[];
    //HVector<double> layers[NUM_LAYERS];
    //HMatrix<double> connections[NUM_LAYERS-1];
    
    int networkDimensions[NUM_LAYERS];
    
    // nodes[layer][nodeid]
    //double* nodes[NUM_LAYERS];
    std::vector<double> nodes[NUM_LAYERS];
    
    // nodes before the sigmoid activation function
    // nodes_Z[layer][nodeid]
    //double* nodesRaw[NUM_LAYERS];
    std::vector<double> nodesRaw[NUM_LAYERS];
    
    // weights between layer1 and layer2
    // weights[layer1][node2id][node1id]
    //double** weights[NUM_LAYERS-1];
    std::vector<std::vector<double>> weights[NUM_LAYERS-1];
    
    // biases at layer
    // biases[layer-1][nodeid]
    //double* biases[NUM_LAYERS-1];
    std::vector<double> biases[NUM_LAYERS-1];
    
    // these are the partials of the cost function (for given training example) with
    // respect to the following variables
    
    // nodes[layer][nodeid]
    //double* nodePartials[NUM_LAYERS];
    std::vector<double> nodePartials[NUM_LAYERS];
    
    // node_Z_Partials is the node values before sigmoid
    //double* nodeRawPartials[NUM_LAYERS];
    std::vector<double> nodeRawPartials[NUM_LAYERS];
    
    // weights between layer1 and layer2
    // weights[layer1][node2id][node1id]
    // double** weightPartials[NUM_LAYERS-1];
    std::vector<std::vector<double>> weightPartials[NUM_LAYERS-1];
    
    // biases at layer
    // biases[layer][nodeid]
    // double* biasPartials[NUM_LAYERS];
    std::vector<double> biasPartials[NUM_LAYERS];
    
    FCAbstractNeuralNetworkTrainer* trainer;
    
public:
    FCNeuralNetwork(const int networkDimensions[NUM_LAYERS]): trainer(nullptr) {
        std::memcpy(this->networkDimensions, networkDimensions, sizeof(int)*NUM_LAYERS);
        for(int layer=INPUT_LAYER; layer<NUM_LAYERS;layer++){
            //nodes[layer] = new double[networkDimensions[layer]];
            nodes[layer].resize(networkDimensions[layer]);
            //nodePartials[layer] = new double[networkDimensions[layer]];
            nodePartials[layer].resize(networkDimensions[layer]);
            
            //nodesRaw[layer] = new double[networkDimensions[layer]];
            nodesRaw[layer].resize(networkDimensions[layer]);
            //nodeRawPartials[layer] = new double[networkDimensions[layer]];
            nodeRawPartials[layer].resize(networkDimensions[layer]);
            
            if(layer!=INPUT_LAYER){
                //biases[layer-1] = new double[networkDimensions[layer]];
                biases[layer-1].resize(networkDimensions[layer]);
                //biasPartials[layer-1] = new double[networkDimensions[layer]];
                biasPartials[layer-1].resize(networkDimensions[layer]);
            }
            if(layer!=OUTPUT_LAYER){//dont run for last layer
                //weights[layer] = new double*[networkDimensions[layer+1]];
                weights[layer].resize(networkDimensions[layer+1]);
                //weightPartials[layer] = new double*[networkDimensions[layer+1]];
                weightPartials[layer].resize(networkDimensions[layer+1]);
                for(int i=0; i <networkDimensions[layer+1];i++){
                    //weights[layer][i] = new double[networkDimensions[layer]];
                    weights[layer][i].resize(networkDimensions[layer]);
                    //weightPartials[layer][i] = new double[networkDimensions[layer]];
                    weightPartials[layer][i].resize(networkDimensions[layer]);
                }
            }
        }
    }
    void setTrainer(const FCAbstractNeuralNetworkTrainer* trainer){
        this->trainer = trainer;
    }
    
    void feed(double* inputLayerData){
        std::memcpy(nodes[INPUT_LAYER].data(), inputLayerData, sizeof(double)*networkDimensions[INPUT_LAYER]);
    }
    void calculateFully(){
        for(int layer=INPUT_LAYER+1; layer <= OUTPUT_LAYER;layer++){
            calculateLayer(layer);
        }
    }
    void calculateLayer(int layer){
        if(layer == INPUT_LAYER){
            throw -1;
        }
        //layers[layer] = connections[layer-1]*layers[layer-1];
        matrixMult(weights[layer-1], nodes[layer-1], networkDimensions[layer], networkDimensions[layer-1], nodesRaw[layer]);
        
        
        double sigmoid(double);
        arrayTransform(nodes[layer], nodesRaw[layer], networkDimensions[layer], sigmoid);
    }
    
    
    //TRAINING
    
    /**
     * Train Intensive uses gradient descent with the entire training examples
     * sample size through a given number of iterations, ie proper theoretical
     * machine learning
     *
     * @param descentIterations
     * @param numExamples
     */
    void trainIntensive(int descentIterations, int numExamples) {
        //this->initPartials(true);
        
        std::vector<std::tuple<double*, double*>>* examples = this->trainer.getRandomTrainingExamples(numExamples);
        for (int i = 0; i < descentIterations; i++) {
            
            //double[] beforePerformance = null;
            /*
            if (i % this->trainer.getN() == 0) {
                beforePerformance = this->calculateAveragePerformance(examples);
            }
             */
            
            this->gradientDescent(examples);
            
            /*
            if (i % this->trainer.getN() == 0) {
                double[] afterPerformance = this->calculateAveragePerformance(examples);
                this->trainer.runEveryNIterations(beforePerformance, afterPerformance);
            }
             */
            
        }
        
        //this->initPartials(false);
    }
    
    /**
     * trainWithBatches is a more optimized machine learning technique that uses a
     * batch of random examples to descend the cost function over and over again,
     * each iteration is a new batch.
     *
     * @param descentIterations
     * @param batchSize
     */
    void trainWithBatches(int descentIterations, int batchSize) {
        //this->initPartials(true);
        
        for (int i = 0; i < descentIterations; i++) {
            std::vector<std::tuple<double*, double*>>* examples = this->trainer.getRandomTrainingExamples(batchSize);
            
            //double[] beforePerformance = nullptr;
            /*
            if (i % this->trainer.getN() == 0) {
                beforePerformance = this->calculateAveragePerformance(examples);
            }
             */
            
            this->gradientDescent(examples);
            
            /*
            if (i % this->trainer.getN() == 0) {
                double[] afterPerformance = this->calculateAveragePerformance(examples);
                
                this->trainer.runEveryNIterations(beforePerformance, afterPerformance);
            }
             */
        }
        
        //this->initPartials(false);
    }
    
    void gradientDescent(std::vector<std::tuple<double*, double*>>* examples) {
        //double[][][] dW = TensorMath.tensorScale(this->weightPartials, 0);
        //double** dW[NUM_LAYERS-1];
        std::vector<std::vector<double>> dW[NUM_LAYERS-1];
        
        //double[][] dB = TensorMath.tensorScale(this->biasPartials, 0);
        //double* dB[NUM_LAYERS-1];
        std::vector<double> dB[NUM_LAYERS-1];
        
        for(int layer=INPUT_LAYER;layer<NUM_LAYERS;layer++){
            if(layer!=OUTPUT_LAYER){//dont run for last layer
                //dW[layer] = new double*[networkDimensions[layer+1]];
                dW[layer].resize(networkDimensions[layer+1]);
                for(int i=0; i <networkDimensions[layer+1];i++){
                    //dW[layer][i] = new double[networkDimensions[layer]];
                    dW[layer][i].resize(networkDimensions[layer]);
                    for(int j=0; j <networkDimensions[layer];j++){
                        dW[layer][i][j] = 0;
                    }
                }
            }
            if(layer!=INPUT_LAYER){
                //dB[layer-1] = new double[networkDimensions[layer]];
                dB[layer-1].resize(networkDimensions[layer]);
                for(int i=0; i <networkDimensions[layer];i++){
                    dB[layer-1][i] = 0;
                }
            }
        }
        
        
        
        for (int exampleI=0; exampleI < examples->size(); exampleI++) {
            std::tuple<double*, double*>& example = ((*examples)[exampleI]);
            auto [d,a] = example;
            this->feed(d);
            this->calculateFully();
            this->calculatePartials(a, true);
            
            //dW = TensorMath::tensorAdd(dW, this->weightPartials);
            addArr2(dW, this->weightPartials, dW);
            //dB = TensorMath::tensorAdd(dB, this->biasPartials);
            addArr(dB, this->biasPartials, dB);
        }
        //dW = TensorMath.tensorScale(dW, -this->trainer.getWeightLearningRate() / 100);
        scaleArr2(dW, -this->trainer.getWeightLearningRate() / 100, dW);
        //dB = TensorMath.tensorScale(dB, -this->trainer.getBiasLearningRate() / 100);
        scaleArr(dB, -this->trainer.getBiasLearningRate() / 100, dB);
        this->takeWeightGradientDescentStep(dW);
        this->takeBiasGradientDescentStep(dB);
    }
    
    void calculatePartials(double* expectedOutput, bool recurseFully) {
        for (int j = 0; j < this->networkDimensions[NUM_LAYERS - 1]; j++) {
            this->nodePartials[NUM_LAYERS - 1][j] = this->trainer.getDerivativeOfCost(this->nodes[NUM_LAYERS - 1][j], expectedOutput[j]);
        }
        if (recurseFully) {
            this->calculatePartials(this->networkDimensions.length - 1, recurseFully);
        }
    }
    
    void calculatePartials(int layer, bool recurseFully) {
        if (layer < 1)
            return;
        for (int i = 0; i < this->networkDimensions[layer]; i++) {
            this->nodeRawPartials[layer][i] = this->nodePartials[layer][i] * sigmoidDerivative(this->nodes_Z[layer][i]);
            for (int k = 0; k < this->networkDimensions[layer - 1]; k++) {
                this->weightPartials[layer - 1][i][k] = this->node_Z_Partials[layer][i] * this->nodes[layer - 1][k];
            }
            this->biasPartials[layer - 1][i] = this->node_Z_Partials[layer][i];
        }
        for (int k = 0; k < this->networkDimensions[layer - 1]; k++) {
            double sum = 0;
            for (int i = 0; i < this->networkDimensions[layer]; i++) {
                sum += this->node_Z_Partials[layer][i] * this->weights[layer - 1][i][k];
            }
            this->nodePartials[layer - 1][k] = sum;
        }
        
        if (recurseFully) {
            this->calculatePartials(layer - 1, recurseFully);
        }
    }
    
    void takeWeightGradientDescentStep(const std::vector<std::vector<double>>& dW) {
        //this->weights = TensorMath.tensorAdd(this->weights, dW);
        addArr2(this->weights, dW);
    }
    
    void takeBiasGradientDescentStep(const std::vector<double>& dB) {
        //this->biases = TensorMath.tensorAdd(this->biases, dB);
        addArr(this->biases, dB);
    }
    
    //performance
    
    /**
     * trainingExamples[trainingExampleNum][0] is the input layer
     * trainingExamples[trainingExampleNum][1] is the expected output layer
     *
     * @param trainingExamples
     * @return the average cost of the network and the average accuracy in
     *         classification
     */
    
    std::tuple<double,double> calculateAveragePerformance(const std::vector<std::tuple<double*, double*>>& trainingExamples) {
        double totalCost = 0, totalAccuracy = 0;
        
        for (int i=0; i <trainingExamples.size();i++) {
            const std::tuple<double*, double*>& example = trainingExamples[i];
            std::tuple<double,bool> performance = this->calculatePerformance(example);
            totalCost += std::get<0>(performance);
            totalAccuracy += (std::get<1>(performance)) ? 1.0 : 0.0;
        }
        double cost = totalCost / trainingExamples.size();
        double accuracy = totalAccuracy / trainingExamples.size();
        return std::make_tuple(cost,accuracy);
    }
    
    /**
     * trainingExamples[0] is the input layer
     *
     * trainingExamples[1] is the expected output layer
     *
     * @param trainingExamples
     * @return the cost of the network ("how bad it is") and boolean if it was
     *         correct result
     */
    std::tuple<double,bool> calculatePerformance(const std::tuple<double*, double*>& trainingExample) {
        this->feed(std::get<0>(trainingExample));
        this->calculateFully();
        double* output = this->getOutput();
        double* expectedOutput = std::get<1>(trainingExample);
        
        double cost = this->trainer.getCost(output, expectedOutput);
        bool correct = this->trainer.isCorrect(output, expectedOutput);
        return std::make_tuple(cost,correct);
    }

    
    
};


void addArr(const std::vector<double>& v1, const std::vector<double>& v2, std::vector<double>& dest){
    
    if(v1.size()!=v2.size() || dest.size() != v1.size()){
        throw -1;
    }
    
    for(int i=0; i <dest.size();i++){
        dest[i] = v1[i]+v2[i];
    }
}
void addArr2(const std::vector<std::vector<double>>& v1, const std::vector<std::vector<double>>& v2, std::vector<std::vector<double>>& dest){
    
    if(v1.size()!=v2.size() || dest.size() != v1.size()){
        throw -1;
    }
    
    for(int i=0; i <dest.size();i++){
        if(v1[i].size()!=v2[i].size() || dest[i].size() != v1[i].size()){
            throw -1;
        }
        for(int j=0; j<dest[i].size();j++){
            dest[i][j] = v1[i][j]+v2[i][j];
        }
    }
}

void scaleArr(const std::vector<double>& v1, double scalar, std::vector<double>& dest){
    
    if(dest.size() != v1.size()){
        throw -1;
    }
    
    for(int i=0; i <dest.size();i++){
        dest[i] = scalar*v1[i];
    }
}
void scaleArr2(const std::vector<std::vector<double>>& v1, double scalar, std::vector<std::vector<double>>& dest){
    
    if(dest.size() != v1.size()){
        throw -1;
    }
    
    for(int i=0; i <dest.size();i++){
        if(dest[i].size() != v1[i].size()){
            throw -1;
        }
        for(int j=0; j<dest[i].size();j++){
            dest[i][j] = scalar*v1[i][j];
        }
    }
}


void vectorDot(double* arr1, double* arr2, int arrsize, double& dest){
    dest = 0;
    for(int i=0; i < arrsize;i++){
        dest += (arr1[i] * arr2[i]);
    }
}
//matrix given row wise, ie matrix[0] is the first row
void matrixMult(std::vector<std::vector<double>> matrix, double* vect, int M,int N, double* destVect){
    for(int m=0; m <M;m++){
        vectorDot(matrix[m].data(), vect, N, destVect[m]);
    }
}
void arrayTransform(double* dest, double* src, int arrsize, double (*transformation)(double)){
    for(int i=0; i <arrsize;i++){
        dest[i] = transformation(src[i]);
    }
}
double sigmoid(double inp){
    double ans = exp(-inp)+1.0;
    ans = 1/ans;
    return ans;
}
double sigmoidDerivative(double inp){
    return sigmoid(inp)*sigmoid(1-inp);
}


#endif /* NeuralNetwork_h */
