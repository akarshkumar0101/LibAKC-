//
// Created by Akarsh Kumar on 1/10/20.
//

#include "Image.h"

#include "stb_image.h"

#include "iostream"

Image::Image(const std::string& fileName) {
    int width, height, numColorChannels;
    stbi_set_flip_vertically_on_load(true);
    mData = stbi_load(fileName.c_str(), &width, &height, &numColorChannels, 0);
    if(mData == nullptr){
        throw std::runtime_error("Could not load image");
    }
    mWidth = width;
    mHeight = height;
    mNumColorChannels = numColorChannels;

    stbi = true;
}
Image::Image(int width, int height, int numColorChannels, unsigned char *data):mWidth(width), mHeight(height), mNumColorChannels(numColorChannels) {
    mData = new unsigned char[width*height*numColorChannels];
    std::memcpy(mData, data, width*height*numColorChannels*sizeof(unsigned char));
    stbi = false;
}
Image::~Image() {
    if(stbi){
        stbi_image_free(mData);
    }
    else{
        delete[] mData;
    }
}

unsigned int Image::getWidth() const{
    return mWidth;
}
unsigned int Image::getHeight()const{
    return mHeight;
}
unsigned int Image::getNumColorChannels()const{
    return mNumColorChannels;
}
unsigned char* Image::getDataPtr()const{
    return mData;
}


