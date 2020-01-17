//
// Created by Akarsh Kumar on 1/11/20.
//

#pragma once

#include <memory>
#include <vector>
#include "src/Array.h"


namespace mnist {

    extern FArray3D trainImages;
    extern Array1D<unsigned char> trainLabels;

    extern FArray3D testImages;
    extern Array1D<unsigned char> testLabels;

    void loadMNISTData(std::string data_root);

//    std::shared_ptr<std::vector<float>> genFloatImage(std::vector<float>& images, int imageID);
}


