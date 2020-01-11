//
// Created by Akarsh Kumar on 11/28/19.
//

#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_set>

#include "Shader.h"
#include "Image.h"

enum BufferUsage{
    STATIC, DYNAMIC
};

class GraphicsBuffer {
protected:
    const unsigned int mBufferID;
    unsigned int mCount; //num elements (indices or vertices)
    size_t mSize; //size of buffer
    virtual GLenum getTarget() const = 0;
public:
    GraphicsBuffer();
    ~GraphicsBuffer();
    void allocateBuffer(const unsigned int count, const size_t size, const void* data = nullptr, BufferUsage usage = STATIC);
    void changeBufferSubData(const size_t offset, const size_t size, const void* data);
    void bind() const;
    void unbind() const;
    unsigned int count() const;
    size_t size() const;
};

class VertexBuffer: public GraphicsBuffer {
private:
    virtual GLenum getTarget() const override {return GL_ARRAY_BUFFER;}
public:
    void allocateBuffer(const unsigned int count, const std::vector<float>& vertexData, BufferUsage usage = STATIC);
};
class IndexBuffer: public GraphicsBuffer {
private:
    virtual GLenum getTarget() const override {return GL_ELEMENT_ARRAY_BUFFER;}
public:
    void allocateBuffer(const std::vector<unsigned int>& indexData, BufferUsage usage = STATIC);
};



struct VertexBufferElement {
    unsigned int type; //represents OpenGL type
    unsigned int count;
    unsigned char normalized;

    static size_t sizeofType(unsigned int type);
};

class VertexBufferLayout {
private:
    //vertex array object
    std::vector<VertexBufferElement> mElements;
    size_t mStride;
public:
    static std::shared_ptr<VertexBufferLayout> coordinate2DOnlyLayout();
    static std::shared_ptr<VertexBufferLayout> coordinate3DOnlyLayout();

    VertexBufferLayout();
    VertexBufferLayout(const std::vector<VertexBufferElement>& elements );

    void addElement(const VertexBufferElement &e);
    void addElement(const unsigned int type, const unsigned int count, const unsigned char normalized);

    template<class T>
    void addElement(const unsigned int count);

    const std::vector<VertexBufferElement>& elements() const;
    size_t stride() const;
};

//
class VertexArray {
private:
    unsigned int mArrayID;
    std::unordered_set<std::shared_ptr<const VertexBuffer>> mReferencedBuffers;
public:
    VertexArray();
    ~VertexArray();

    // uses default layout
    void addBufferWithDefaultLayout(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout);
    // uses shader layout
    void addBufferWithShaderAttributes(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout, const Shader& shader, const std::vector<std::string>& attributeNames);

    std::unordered_set<std::shared_ptr<const VertexBuffer>>& referencedBuffers(){
        return mReferencedBuffers;
    }

    void bind() const;
    void unbind() const;
};


class GraphicsData {
private:
    std::shared_ptr<VertexArray> mVertexArray;
    std::shared_ptr<IndexBuffer> mIndexBuffer;

    void loadFromFile(const std::string &fileName);
    void initBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

public:
    GraphicsData(const std::string &fileName);
    GraphicsData(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<IndexBuffer> indexBuffer);

    GraphicsData(const std::vector<float> &vertices);
    GraphicsData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);

//    GraphicsData(const RigidBody &model);

    ~GraphicsData();

    std::shared_ptr<IndexBuffer> indexBuffer() const{
        return mIndexBuffer;
    }

    void bind() const;
    void unbind() const;

};
namespace CommonModels{
    std::shared_ptr<GraphicsData> teapotModel();
    std::shared_ptr<GraphicsData> squareModel();
    std::shared_ptr<GraphicsData> cubeModel();
    std::shared_ptr<GraphicsData> arrowModel();
}



class Texture{
private:
    const unsigned int mBufferID;
public:
    Texture();
    ~Texture();

    void allocateTexture(const Image& image);

    void bind();
    void unbind();
};

