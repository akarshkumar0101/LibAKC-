#include <iostream>
#include "src/machinelearning/Node.h"
#include "src/machinelearning/LayeredNeuralNetwork.h"
#include "src/machinelearning/Layer.h"
#include "mnist.h"
#include "src/machinelearning/NeuralNetwork.h"
#include "src/Array.h"

#include <vector>
#include <memory>
#include <fstream>
#include "src/graphics/Window.h"
#include "src/graphics/ImagePane.h"

#include <GLFW/glfw3.h>
#include <random>
#include "src/graphics/GridPane.h"

int getPredictedDigit(std::vector<float>& networkOut){
    float maxSeen = -100.0f;
    int d=-1;
    for(int i=0;i<networkOut.size();i++){
        if(networkOut[i]>maxSeen){
            maxSeen = networkOut[i];
            d = i;
        }
    }
    return d;
}
std::vector<float> getExpectedOutput(unsigned int digit){
    std::vector<float> expout(10,0.0f);
    expout[digit] = 1.0f;
    return expout;
}

void printCost(NeuralNetwork<FArray2D, FArray1D >& nn, FArray3D& images, Array1D<unsigned char> labels){
    float cost = 0.0f;
    int amountRight = 0;
    for(int i=0;i<images.size();i++){
        auto input = std::make_shared<FArray2D>(images[i]);
        auto output = nn.compute(input);
        int digitout = getPredictedDigit(*output);
        if(digitout == labels[i]){
            amountRight++;
        }
//        std::cout<<nn.costRegime.costFunction(*output, getExpectedOutput(labels[i]))<<std::endl;
        cost+=nn.costRegime.costFunction(*output, getExpectedOutput(labels[i]));
    }
    std::cout<<"cost: "<<cost<<", accuracy: "<<(float)amountRight/images.size()<<std::endl;
}
void train(NeuralNetwork<FArray2D, FArray1D >& nn, FArray3D& images, Array1D<unsigned char> labels, unsigned int batchSize, unsigned int epochs){
    int numBatches = images.size()/batchSize;

    for(int epoch = 0; epoch< epochs;epoch ++) {

        for (int batchNum = 0; batchNum < numBatches; batchNum++) {
            std::cout<<batchNum<<"/"<<numBatches<<std::endl;
            int numCorrect= 0;

            for (int ex = 0; ex < batchSize; ex++) {
                //create new custom input
                auto input = std::make_shared<FArray2D>(images[batchNum * batchSize + ex]);
                auto output = nn.compute(input);
                auto digit = getPredictedDigit(*output);
                auto expout = getExpectedOutput(labels[batchNum * batchSize + ex]);
                nn.backprop(expout);

                if(digit==labels[batchNum*batchSize+ex]){
                    numCorrect++;
                }

            }
            nn.takeStep();

            std::cout<<(float) numCorrect/batchSize<<std::endl;

            if(batchNum%100==0)
                printCost(nn,mnist::testImages, mnist::testLabels);
        }
        std::cout<<"Epoch "<<epoch<<" done."<<std::endl;
    }

}





int main() {


    srand(100);

    mnist::loadMNISTData("/Users/akarshkumar0101/Datasets/MNIST/");

//    int numTrainImages = 1000, numTestImages = 1000;
//    auto trainImages = std::make_shared<FArray3D>(mnist::trainImages.begin(), mnist::trainImages.begin()+numTrainImages);
//    auto trainLabels = std::make_shared<Array1D<unsigned char>>(mnist::trainLabels.begin(), mnist::trainLabels.begin()+numTrainImages);
//    auto testImages = std::make_shared<FArray3D>(mnist::testImages.begin(), mnist::testImages.begin()+numTestImages);
//    auto testLabels = std::make_shared<Array1D<unsigned char>>(mnist::testLabels.begin(), mnist::testLabels.begin()+numTestImages);


    NeuralNetwork<FArray2D, FArray1D> nn;
    nn.setCostRegime(squareErrorRegime);

    nn.emplaceLayer<Flatten2D>(28,28);
    nn.emplaceLayer<DenseLayer>(784,100,sigmoidActivationFunction);
    nn.emplaceLayer<DenseLayer>(100,10,sigmoidActivationFunction);


    train(nn, mnist::trainImages, mnist::trainLabels, 100, 1);
    return 0;

    Conv2DLayer layer(1, 3, 3, 1);
    FArray3D& kernel = layer.mKernels.front();
    kernel[0] = {{-1.0f/4, -2.0f/4, -1.0f/4},
                 {0.0f, 0.0f, 0.0f},
                 {1.0f/4, 2.0f/4, 1.0f/4}};

    FArray2D img2D = mnist::trainImages[2];
    auto img1_ = std::make_shared<FArray3D> (1, img2D);

    auto conv = layer.compute(img1_);
    FArray2D conv_ = conv->front();

    for(auto& row:conv_){
        for(auto &pix:row){
            pix = std::max(pix,0.0f);
            pix = std::min(pix,1.0f);
        }
    }


    return 0;

    auto window = std::make_shared<Window>(500,500,"hey");

//    Image img(mnist::imageWidth,mnist::imageHeight,1, mnist::testImages->data()+(28*28*100));
    auto gridPane = std::make_shared<GridPane>(window, std::pair{6,1});

    std::vector<std::shared_ptr<ImagePane>> imagePanes(6);
    for(int i=0;i<6;i++) {
        imagePanes[i] = std::make_shared<ImagePane>(window);
        gridPane->setChild(imagePanes[i],{i,0});
    }

    window->setRoot(gridPane);

    int batch=-1;
    bool isPressed = false;

    while(!window->shouldClose()){

        if(window->isKeyPressed(GLFW_KEY_SPACE) && !isPressed){
            isPressed = true;
            batch++;

            for(int i=0;i<6;i++){
                Image img(mnist::trainImages[batch*6+i]);
                imagePanes[i]->setImage(img);

                std::cout<<(int) mnist::trainLabels.at(batch*6+i)<<", ";
            }
            std::cout<<std::endl;

            Image raw(img2D);
            Image convImg(conv_);
//            imagePanes[0]->setImage(raw);
//            imagePanes[1]->setImage(convImg);

        }
        if(!(window->isKeyPressed(GLFW_KEY_SPACE))){
            isPressed = false;
        }


        window->render();

        window->swapBuffers();
        window->pollEvents();
    }

//    DenseLayer dl(10,5, nullptr);
//
//
//    LayeredNeuralNetwork net(input);
//    net.addLayer(3);
//    net.addLayer(3);
//    net.addLayer(7);
//
//    net.computeFully();
//
//    const std::vector<float>& out = net.output();
//
//    for(float f: out){
//        std::cout<<f<<std::endl;
//    }

    return 0;
}


