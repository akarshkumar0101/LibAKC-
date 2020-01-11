//
//  main.cpp
//  LibAKC++
//
//  Created by Akarsh Kumar on 4/3/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "byte_operations.hpp"


#include "HMatrix.h"
#include "HVector.h"
#include "FCNeuralNetwork.h"
#include "FCNeuralNetworkTrainer.h"

class Animal {
public:
    std::string name;
    int numLegs;
    Animal(): Animal("unnamed", 0){
    }
    Animal(std::string name, int numLegs){
        this->name = name;
        this->numLegs = numLegs;
    }
    Animal(const Animal& animal){
        //this->name = animal.name;
        //this->numLegs = animal.numLegs;
    }
    Animal& operator=(const Animal& animal) {
        //this->name = animal.name;
        this->numLegs = animal.numLegs;
        return *this;
    }
    
    void makeSound(){
        std::cout<<"Animal Talking" <<std::endl;
    }
};
std::ostream& operator<<(std::ostream& os, const Animal& animal)
{
    os << animal.name << " with "<< animal.numLegs<< " legs. ";
    return os;
}

template <class T, int M, int N>
std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& matrix)
{
    
    for(int m = 0;m <M;m++){
        for(int n=0; n<N;n++){
            os<<matrix.get(m,n)<<" ";
        }
        os<<std::endl;
    }
    return os;
}

int main(int argc, const char * argv[]) {
    int arr[4] = {784, 16, 16, 10};
    FCNeuralNetwork<4> network(arr);
    
    class DigitTrainer: public FCNeuralNetworkTrainer<int*, int>{
    public:
        void rawToInputLayer(int* const& d, double* inputLayer, const int inputLayerSize) {
        }
        
        void rawToOutputLayer(const int& a, double* outputLayer, const int outputLayerSize) {
        }
        
        std::tuple<int*, int> getRandomRawData(){
            return std::make_tuple(nullptr, 3);
        }
        
        double getWeightLearningRate(){
            return 1.0;
        }
    };
    
    DigitTrainer trainer;
    
    
    return 0;
}




int mainf(int argc, const char * argv[]) {
    // insert code here...
    
    std::ifstream instreamLabels;
    std::ifstream instreamImages;
    //instream.open("../MNIST\ Dataset/train-labels-idx1-ubyte.gz");
    instreamLabels.open("/Users/akarshkumar0101/Developer/MNIST Dataset/train-labels-idx1-ubyte");
    instreamImages.open("/Users/akarshkumar0101/Developer/MNIST Dataset/train-images-idx3-ubyte");
    
    if(instreamLabels.is_open()) {
        uint32_t magicNumberLabel;
        instreamLabels.read((char*)&magicNumberLabel, 4);
        magicNumberLabel = reverseByteOrder32(magicNumberLabel);
        std::cout << (int) magicNumberLabel << std::endl;
        
        uint32_t magicNumberImage;
        instreamLabels.read((char*)&magicNumberImage, 4);
        magicNumberImage = reverseByteOrder32(magicNumberImage);
        std::cout << (int) magicNumberImage << std::endl;

        uint32_t numLabels;
        instreamImages.read((char*)&numLabels, 4);
        numLabels = reverseByteOrder32(numLabels);
        std::cout << (int) numLabels << std::endl;
        
        uint32_t numImages;
        instreamImages.read((char*)&numImages, 4);
        numImages = reverseByteOrder32(numImages);
        std::cout << (int) numImages << std::endl;
        
        
        uint32_t numRows, numCols;
        instreamImages.read((char*)&numRows, 4);
        numRows = reverseByteOrder32(numRows);
        instreamImages.read((char*)&numCols, 4);
        numCols = reverseByteOrder32(numCols);
        
        std::cout << "rows: " << (int) numRows<<std::endl;
        std::cout << "rows: " << (int) numCols<<std::endl;
        
        
        while(!instreamLabels.eof()) {
            uint8_t label;
            instreamLabels.read((char*)&label, 1);
            
            std::cout<<"label: "<<(int)label<<std::endl;
            for(int row=0; row<(int) numRows;row++) {
                for(int col=0; col<(int) numCols;col++) {
                    uint8_t pixel;
                    instreamImages.read((char*)&pixel, 1);
                    
                    if(((int) pixel)>120) {
                        std::cout<<"*";
                    }
                    else {
                        std::cout<<" ";
                    }
                }
                std::cout<<std::endl;
            }
            
            std::cout<<std::endl;
            std::cout<<std::endl;
            
            
        }
    }
    
    
    
    
    instreamLabels.close();
    instreamImages.close();
    return 0;
}



