//
// Created by Akarsh Kumar on 1/11/20.
//

#include "mnist.h"

#include <fstream>
#include <iostream>


namespace mnist {

    FArray3D trainImages;
    Array1D<unsigned char> trainLabels;

    FArray3D testImages;
    Array1D<unsigned char> testLabels;

    static int readBigEndianInt(std::ifstream& IFS){
        char idata[4];
        IFS.read(idata+3,1);
        IFS.read(idata+2,1);
        IFS.read(idata+1,1);
        IFS.read(idata,1);
        return *((int*)idata);
    }
    static void loadMNISTImages(std::string path, int realMagicNumber, FArray3D& dataSet){
        std::ifstream imageIFS(path, std::ios::binary);

        int magicNumber = readBigEndianInt(imageIFS);
        int numImages = readBigEndianInt(imageIFS);
        int height = readBigEndianInt(imageIFS);
        int width = readBigEndianInt(imageIFS);

        if(magicNumber != realMagicNumber){
            throw std::runtime_error("Magic number not read properly, something went wrong");
        }

        dataSet.resize(numImages);

        for(int img=0;img<numImages;img++){
            dataSet[img].resize(height);
            for(int y=0;y<height;y++){
                dataSet[img][y].resize(width);
                for(int x=0; x < width; x++){
                    char pix;
                    imageIFS.read(&pix, 1);
                    dataSet[img][y][x] = ((unsigned char)pix) / 255.0f;
                }
            }
        }

//        char lastByte;
//        imageIFS.read(&lastByte, 1);
    }
    static void loadMNISTLabels(std::string path, int magicNumber, Array1D<unsigned char>& dataSet){
        std::ifstream labelIFS(path, std::ios::binary);

        int mn = readBigEndianInt(labelIFS);
        int numImages = readBigEndianInt(labelIFS);

        if(mn != magicNumber){
            throw std::runtime_error("Magic number not read properly, something went wrong");
        }

        dataSet.resize(numImages);

        for(int i=0;i<numImages;i++){
            char label;
            labelIFS.read(&label, 1);
            dataSet[i] = label;
        }
//        char lastByte;
//        labelIFS.read(&lastByte, 1);
    }


    void loadMNISTData(std::string data_root){
        loadMNISTLabels(data_root+"/train-labels-idx1-ubyte", 2049, trainLabels);
        loadMNISTImages(data_root+"/train-images-idx3-ubyte", 2051, trainImages);

        loadMNISTLabels(data_root+"/t10k-labels-idx1-ubyte", 2049, testLabels);
        loadMNISTImages(data_root+"/t10k-images-idx3-ubyte", 2051, testImages);
    }
}

//    using Row = std::vector<unsigned char>;
//    using Img = std::vector<Row>;
//    using ImgPtr = std::shared_ptr<Img>;
//    using DataSet = std::vector<ImgPtr>;
//    using DataSetPtr = std::shared_ptr<DataSet>;
//
//    DataSetPtr dsp = std::make_shared<DataSet>(numImages);
//    for(int image=0;image<numImages;image++){
//        (*dsp)[image] = std::make_shared<Img>(height, std::vector<unsigned char>(width));
//    }
//    std::cout<<dsp->size()<<std::endl;
//    std::cout<<(*dsp)[0]->size()<<std::endl;
//    std::cout<<(*(*dsp)[0])[0].size()<<std::endl;
