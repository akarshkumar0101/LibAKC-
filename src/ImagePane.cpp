//
// Created by Akarsh Kumar on 1/10/20.
//

#include "ImagePane.h"
#include "stb_image.h"


static Shader& imageShader(){
    static Shader imageShader("resources/shaders/image.shader");
    return imageShader;
}
static VertexArray& vertexArray() {
    static bool first = true;
    static std::shared_ptr<VertexBuffer> vb(std::make_shared<VertexBuffer>());
    static VertexArray va;
    static VertexBufferLayout vbl;

    if (first) {
        first = false;

        vb->allocateBuffer(4, {1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
                               1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
                               -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
                               -1.0f, 1.0f, 0.0f, 0.0f, 1.0f});

        vbl.addElement<float>(3);
        vbl.addElement<float>(2);
        va.addBufferWithDefaultLayout(vb, vbl);
    }
    return va;
}
static IndexBuffer& indexBuffer(){
    static bool first = true;
    static IndexBuffer ib;
    if(first){
        first = false;
        ib.allocateBuffer({0,1,2,0,2,3});
    }
    return ib;
}

ImagePane::ImagePane(std::shared_ptr<Window> window, const Image& image): Pane(window), mTexture(std::make_shared<Texture>()) {
    setImage(image);
}
ImagePane::~ImagePane(){

}
void ImagePane::setImage(const Image& image){
    mTexture->allocateTexture(image);
}

void ImagePane::render(const Viewport &viewport) {
    viewport.bind();

    mTexture->bind();
    imageShader().bind();
    vertexArray().bind();
    indexBuffer().bind();
    glDrawElements(GL_TRIANGLES,indexBuffer().count(),GL_UNSIGNED_INT, nullptr);
}


