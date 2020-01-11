//
// Created by Akarsh Kumar on 1/10/20.
//

#pragma once

#include <string>


class Image {
private:
    unsigned int mWidth, mHeight, mNumColorChannels;
    unsigned char* mData;

    bool stbi;
public:
    Image(const std::string& fileName);
    Image(int width, int height, int numColorChannels, unsigned char* data);
    ~Image();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getNumColorChannels() const;
    unsigned char* getDataPtr() const;
};


