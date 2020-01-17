//
// Created by Akarsh Kumar on 1/10/20.
//

#include "Image.h"

#include "stb_image.h"

#include "iostream"

Image::Image(const std::string& fileName) {
    int width, height, numColorChannels;
//    stbi_set_flip_vertically_on_load(true);
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
    mData = new unsigned char[mWidth*mHeight*mNumColorChannels];
    std::memcpy(mData, data, width*height*numColorChannels*sizeof(unsigned char));
    stbi = false;
}

Image::Image(int width, int height, int numColorChannels, float* data):mWidth(width), mHeight(height), mNumColorChannels(numColorChannels) {
    mData = new unsigned char[mWidth*mHeight*mNumColorChannels];
    for(int i=0;i<width*height*numColorChannels;i++){
        mData[i] = (unsigned char) (data[i]*255.0f);
    }
    stbi = false;
}
Image::Image(FArray2D& array2D):mWidth(array2D.front().size()), mHeight(array2D.size()), mNumColorChannels(1) {
    mData = new unsigned char[mWidth*mHeight*mNumColorChannels];
    int i=0;
    for(int y=0; y < mHeight; y++){
        for(int x=0;x<mWidth;x++){
            mData[i++] = (unsigned char) (array2D[y][x]*255.0f);
        }
    }
    stbi = false;
}
Image::Image(FArray3D& array3D):mWidth(array3D.front().size()), mHeight(array3D.size()), mNumColorChannels(array3D.front().front().size()) {
    mData = new unsigned char[mWidth*mHeight*mNumColorChannels];
    int i=0;
    for(int n=0;n<mNumColorChannels;n++) {
        for (int y = 0; y < mHeight; y++) {
            for (int x = 0; x < mWidth; x++) {
                mData[i++] = (unsigned char) (array3D[y][x][n] * 255.0f);
            }
        }
    }
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


