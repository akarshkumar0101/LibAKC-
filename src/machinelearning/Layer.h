//
// Created by Akarsh Kumar on 1/10/20.
//

#pragma once

#include <memory>
#include <vector>

#include "Node.h"
#include "../Array.h"

class LayerRaw {
private:
public:
    virtual std::shared_ptr<void> compute(std::shared_ptr<void> input) = 0;
    virtual std::shared_ptr<void> backprop(std::shared_ptr<void> nodePartials) = 0;
    virtual void takeStep() =0;
};

template<class I, class O>
class Layer: public LayerRaw {
private:
public:
    std::shared_ptr<void> compute(std::shared_ptr<void> input) override{
        std::shared_ptr<I> castedInput = std::static_pointer_cast<I>(input);
        // does not recurse infinitely, calls the other virtual compute function
        return compute(castedInput);
    }
    std::shared_ptr<void> backprop(std::shared_ptr<void> nodePartials) override{
        std::shared_ptr<O> casted = std::static_pointer_cast<O>(nodePartials);
        // does not recurse infinitely, calls the other virtual compute function
        return backprop(casted);
    }

    virtual std::shared_ptr<O> compute(std::shared_ptr<I> input) = 0;
    virtual std::shared_ptr<I> backprop(std::shared_ptr<O> nodePartials) = 0;
};

// float[] to float[]
class DenseLayer: public Layer<FArray1D, FArray1D> {
private:
    ActivationFunction mActivationFunction;

    const int mNumNodesPrev, mNumNodes;
    FArray2D mWeights;
    FArray1D mBiases;

    // gradient descent stuff
    FArray1D mBiasPartials;
    FArray2D mWeightPartials;

    FArray1D mPreviousZ;
    std::shared_ptr<FArray1D> mPreviousInput;


public:
    DenseLayer(int numNodesPrev, int numNodes, ActivationFunction activationFunction);

    std::shared_ptr<FArray1D> compute(std::shared_ptr<FArray1D> input) override;

    std::shared_ptr<FArray1D> backprop(std::shared_ptr<FArray1D> nodePartials) override;

    void takeStep() override;
};
class Flatten2D: public Layer<FArray2D, FArray1D>{
private:
    unsigned int mHeight, mWidth;
public:
    Flatten2D(unsigned int height, unsigned int width): mHeight(height), mWidth(width) {}
    std::shared_ptr<FArray1D> compute(std::shared_ptr<FArray2D> input) override{
        auto out = newArray1D(mHeight*mWidth, 0.0f);
        int i=0;
        for(auto& row: *input){
            for(auto& pix: row){
                (*out)[i++] = pix;
            }
        }
        return out;
    };

    std::shared_ptr<FArray2D> backprop(std::shared_ptr<FArray1D> nodePartials) override{
        auto out = newArray2D(mHeight,mWidth, 0.0f);
        int i=0;
        for(int y=0;y<mHeight;y++){
            for(int x=0;x<mWidth;x++){
                (*out)[y][x] = (*nodePartials)[i++];
            }
        }
        return out;
    };

    void takeStep() override{};
};
class Flatten3D: public Layer<FArray3D, FArray1D>{
private:
    unsigned int mImages, mHeight, mWidth;
public:
    Flatten3D(unsigned int images, unsigned int height, unsigned int width): mImages(images), mHeight(height), mWidth(width) {}

    std::shared_ptr<FArray1D> compute(std::shared_ptr<FArray3D> input) override{
        auto out = newArray1D(mImages*mHeight*mWidth, 0.0f);
        int i=0;
        for(auto& image: *input) {
            for (auto &row: image) {
                for (auto &pix: row) {
                    (*out)[i++] = pix;
                }
            }
        }
        return out;
    };

    std::shared_ptr<FArray3D> backprop(std::shared_ptr<FArray1D> nodePartials) override{
        auto out = newArray3D(mImages, mHeight,mWidth, 0.0f);
        int i=0;
        for(int image=0;image<mImages;image++) {
            for (int y = 0; y < mHeight; y++) {
                for (int x = 0; x < mWidth; x++) {
                    (*out)[image][y][x] = (*nodePartials)[i++];
                }
            }
        }
        return out;
    };

    void takeStep() override{};
};


class Conv2DLayer: public Layer<FArray3D, FArray3D> {
private:
public:
    std::vector<FArray3D> mKernels;

    std::vector<FArray3D> mKernelPartials;

    std::shared_ptr<FArray3D> previousInput;

    float convolveUnit(const FArray3D& kernel, const FArray3D& input, int imgy, int imgx){
        float c = 0.0f;
        int kernelHeight = kernel.front().size(), kernelWidth = kernel.front().front().size();

        for(int kernelDepth=0; kernelDepth<kernel.size(); kernelDepth++) {
            for (int t = -kernelHeight / 2; t <= kernelHeight / 2; t++) {
                if (imgy + t < 0 || imgy + t >= input.front().size()) {
                    continue;
                }
                for (int s = -kernelWidth / 2; s <= kernelWidth / 2; s++) {
                    if (imgx + s < 0 || imgx + s >= input.front().front().size()) {
                        continue;
                    }
                    c += kernel[kernelDepth][t + kernelHeight / 2][s + kernelWidth / 2] * input[kernelDepth][imgy + t][imgx + s];
                }
            }
        }
        return c;
    }
public:
    //numImages is kernal depth
    Conv2DLayer(unsigned int numKernals, unsigned int kernalHeight, unsigned int kernalWidth, unsigned int numImages): mKernels(numKernals, *newArray3D(numImages, kernalHeight, kernalWidth, 0.0f)) {
        if(kernalHeight%2==0||kernalWidth%2==0){
            throw std::runtime_error("Cannot have even kernal height or width");
        }
    }

    std::shared_ptr<FArray3D> compute(std::shared_ptr<FArray3D> input) override{
        if(input->empty() || input->front().empty() || input->front().front().empty()){
            throw std::runtime_error("Invalid input, one dimension is empty");
        }
        unsigned int imageHeight = input->front().size();
        unsigned int imageWidth = input->front().front().size();

        previousInput = input;

        auto output = newArray3D(mKernels.size(), imageHeight, imageWidth, 0.0f);

        for(int kernelID =0; kernelID < mKernels.size(); kernelID++){
            const FArray3D& kernal = mKernels[kernelID];
            for(int imgy=0; imgy < imageHeight; imgy++){
                for(int imgx=0; imgx < imageWidth; imgx++){
                    // convolve kernel with the input;
                    float c = convolveUnit(kernal, *input, imgy, imgx);
                    (*output)[kernelID][imgy][imgx] = c;
                }
            }
        }
        return output;
    }
    std::shared_ptr<FArray3D> backprop(std::shared_ptr<FArray3D> nodePartials) override {
        auto output = std::make_shared<FArray3D>(/*TODO*/);
        int width, height;
        int kernelHeight = mKernels.front().front().size(), kernelWidth = mKernels.front().front().front().size();

        for(int j=0;j<mKernelPartials.size();j++){ //num kernels/ output images
            for(int i=0;i<mKernelPartials[j].size();i++){ // kernel depth/ input images
                for (int t = -kernelHeight / 2; t <= kernelHeight / 2; t++) {
                    for (int s = -kernelWidth / 2; s <= kernelWidth / 2; s++) {
                        float sum =0.0f;
                        //calculate sum
                        for(int imgy=0; imgy < height; imgy++){
                            if (imgy + t < 0 || imgy + t >= previousInput->front().size()) {
                                continue;
                            }
                            for(int imgx=0; imgx < width; imgx++){
                                if (imgx + s < 0 || imgx + s >= previousInput->front().front().size()) {
                                    continue;
                                }
                                sum += (*nodePartials)[j][imgy][imgx] * (*previousInput)[i][imgy + t][imgx + s];
                            }
                        }
                        mKernelPartials[j][i][t+kernelHeight/2][s+kernelWidth/2] = sum;
                    }
                }
            }
        }

        for(int i=0;i<mKernelPartials.front().size();i++) { // kernel depth/ input images
            for(int imgy=0; imgy < height; imgy++) {
                for (int imgx = 0; imgx < width; imgx++) {
                    float sum = 0.0f;
                    //calculate sum
                    for(int j=0;j<mKernelPartials.size();j++) { //num kernels/ output images
                        for (int t = -kernelHeight / 2; t <= kernelHeight / 2; t++) {
                            if (imgy + t < 0 || imgy + t >= previousInput->front().size()) {
                                continue;
                            }
                            for (int s = -kernelWidth / 2; s <= kernelWidth / 2; s++) {
                                if (imgx + s < 0 || imgx + s >= previousInput->front().front().size()) {
                                    continue;
                                }
                                sum+= (*nodePartials)[j][imgy][imgx] * mKernels[j][i][t+kernelHeight/2][s+kernelWidth/2];
                            }
                        }
                    }

                    (*output)[i][imgy][imgx] = sum;
                }
            }
        }
        return output;
    }

    void takeStep() override{
        int kernelHeight = mKernels.front().front().size(), kernelWidth = mKernels.front().front().front().size();
        for(int j=0;j<mKernelPartials.size();j++) { //num kernels/ output images
            for(int i=0;i<mKernelPartials[j].size();i++) { // kernel depth/ input images
                for (int t = -kernelHeight / 2; t <= kernelHeight / 2; t++) {
                    for (int s = -kernelWidth / 2; s <= kernelWidth / 2; s++) {
                        mKernels[j][i][t+kernelHeight/2][s+kernelWidth/2] += mKernelPartials[j][i][t+kernelHeight/2][s+kernelWidth/2];
                        //reset gradient
                        mKernelPartials[j][i][t+kernelHeight/2][s+kernelWidth/2] = 0.0f;
                    }
                }
            }
        }
    }

};
//
//class FlattenLayer: public Layer {
//
//};
