//
// Created by Akarsh Kumar on 1/10/20.
//

#pragma once

#include "Pane.h"
#include "Image.h"
#include "GraphicsBuffer.h"


class ImagePane: public Pane {
private:
//    std::shared_ptr<Image> mImage;
    std::shared_ptr<Texture> mTexture;
public:
    ImagePane(std::shared_ptr<Window> window, const Image& image);
    ~ImagePane();

    void setImage(const Image& image);

    void render(const Viewport& viewport) override;
};


