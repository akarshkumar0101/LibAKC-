//
// Created by Akarsh Kumar on 1/10/20.
//

#pragma once

#include <string>
#include "../Array.h"


class Image {
private:
    unsigned int mWidth, mHeight, mNumColorChannels;
    unsigned char* mData;

    bool stbi;
public:
    Image(const std::string& fileName);
    Image(int width, int height, int numColorChannels, unsigned char* data);
    Image(int width, int height, int numColorChannels, float* data);
    Image(FArray2D& array2D);
    // array3d index with [y][x][channel]
    Image(FArray3D& array3D);
    ~Image();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getNumColorChannels() const;
    unsigned char* getDataPtr() const;
};


