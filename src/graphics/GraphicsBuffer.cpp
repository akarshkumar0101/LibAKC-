//
// Created by Akarsh Kumar on 12/11/19.
//

#include <fstream>
#include "GraphicsBuffer.h"
#include "Image.h"

static GLenum toGLBufferUsage(BufferUsage usage){
    switch(usage) {
        case STATIC:return GL_STATIC_DRAW;
        case DYNAMIC:return GL_DYNAMIC_DRAW;
    }
}

static unsigned int generateBufferOpenGLBuffer(){ unsigned int bufferID; glGenBuffers(1, &bufferID); return bufferID; }
static int bufferCountf = 0;
GraphicsBuffer::GraphicsBuffer():mBufferID(generateBufferOpenGLBuffer()), mSize(0), mCount(0) {
    bufferCountf++;
}
GraphicsBuffer::~GraphicsBuffer(){
    glDeleteBuffers(1, &mBufferID);
    bufferCountf--;
}
void GraphicsBuffer::allocateBuffer(const unsigned int count, const size_t size, const void* data, BufferUsage usage){
    bind();
    glBufferData(getTarget(), size, data, toGLBufferUsage(usage));
    mSize = size;
    mCount = count;
    unbind();
}
void GraphicsBuffer::changeBufferSubData(const size_t offset, const size_t size, const void* data){
    bind();
    glBufferSubData(getTarget(), offset, size, data);
    unbind();
}
void GraphicsBuffer::bind() const{glBindBuffer(getTarget(), mBufferID);}
void GraphicsBuffer::unbind() const{glBindBuffer(getTarget(), 0);}
unsigned int GraphicsBuffer::count() const{return mCount;}
size_t GraphicsBuffer::size() const{return mSize;}


void VertexBuffer::allocateBuffer(const unsigned int count, const std::vector<float> &vertexData, BufferUsage usage) {
    GraphicsBuffer::allocateBuffer(count, vertexData.size()*sizeof(float), vertexData.data(), usage);
}
void IndexBuffer::allocateBuffer(const std::vector<unsigned int> &indexData, BufferUsage usage) {
    GraphicsBuffer::allocateBuffer(indexData.size(), indexData.size()*sizeof(unsigned int), indexData.data(), usage);
}

size_t VertexBufferElement::sizeofType(unsigned int type){
    switch(type){
        case GL_FLOAT: return sizeof(float);
        case GL_UNSIGNED_INT: return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
    }
    return 0;
}


VertexBufferLayout::VertexBufferLayout():mStride(0){
}
VertexBufferLayout::VertexBufferLayout(const std::vector<VertexBufferElement>& elements ):VertexBufferLayout(){
    for(auto e:elements){
        addElement(e);
    }
}
void VertexBufferLayout::addElement(const VertexBufferElement &e){
    mStride += e.count * VertexBufferElement::sizeofType(e.type);
    mElements.push_back(e);
}
void VertexBufferLayout::addElement(const unsigned int type,const unsigned int count, const unsigned char normalized){
    addElement({type, count, normalized});
}

template<>
void VertexBufferLayout::addElement<float>(unsigned int count){
    addElement(GL_FLOAT, count, GL_FALSE);
}
template<>
void VertexBufferLayout::addElement<unsigned int>(unsigned int count){
    addElement(GL_UNSIGNED_INT, count,GL_FALSE);
}
template<>
void VertexBufferLayout::addElement<unsigned char>(unsigned int count){
    addElement(GL_UNSIGNED_BYTE, count, GL_FALSE);
}

std::shared_ptr<VertexBufferLayout> VertexBufferLayout::coordinate2DOnlyLayout(){
    static auto vbl = std::make_shared<VertexBufferLayout>(std::vector<VertexBufferElement>({{GL_FLOAT, 2, false}}));
    return vbl;
}

std::shared_ptr<VertexBufferLayout> VertexBufferLayout::coordinate3DOnlyLayout(){
    static auto vbl = std::make_shared<VertexBufferLayout>(std::vector<VertexBufferElement>({{GL_FLOAT, 3, false}}));
    return vbl;
}

const std::vector<VertexBufferElement> &VertexBufferLayout::elements() const{
    return mElements;
}
size_t VertexBufferLayout::stride() const{
    return mStride;
}

VertexArray::VertexArray(){
    glGenVertexArrays(1, &mArrayID);
}
VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &mArrayID);
}

void VertexArray::addBufferWithDefaultLayout(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout){
//    mReferencedBuffers.insert(std::shared_ptr<VertexBuffer>(&vertexBuffer));
    mReferencedBuffers.insert(vertexBuffer);

    bind();
    vertexBuffer->bind();

    size_t offset = 0;

    for(unsigned int i=0; i < vertexBufferLayout.elements().size(); i++){
        const VertexBufferElement& vertexBufferElement = vertexBufferLayout.elements().at(i);
        glVertexAttribPointer(i, vertexBufferElement.count, vertexBufferElement.type, vertexBufferElement.normalized,
                              vertexBufferLayout.stride(), (void *) offset);
        glEnableVertexAttribArray(i);

        offset += vertexBufferElement.count * VertexBufferElement::sizeofType(vertexBufferElement.type);
    }
}


void VertexArray::addBufferWithShaderAttributes(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout, const Shader& shader, const std::vector<std::string>& attributeNames){
    mReferencedBuffers.insert(vertexBuffer);
    bind();
    vertexBuffer->bind();

    size_t offset = 0;

    for(unsigned int i=0; i < vertexBufferLayout.elements().size(); i++){
        const VertexBufferElement& vertexBufferElement = vertexBufferLayout.elements().at(i);
        int attribLocation = shader.getAttribLocation(attributeNames.at(i));
        glVertexAttribPointer(attribLocation, vertexBufferElement.count, vertexBufferElement.type, vertexBufferElement.normalized,
                              vertexBufferLayout.stride(), (void *) offset);
        glEnableVertexAttribArray(attribLocation);

        offset += vertexBufferElement.count * VertexBufferElement::sizeofType(vertexBufferElement.type);
    }
}

void VertexArray::bind() const{
    glBindVertexArray(mArrayID);
}
void VertexArray::unbind() const{
    glBindVertexArray(0);
}

void GraphicsData::loadFromFile(const std::string &fileName) {
    std::ifstream fileStream(fileName);
    std::string word;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    while (fileStream >> word) {
        if (word == "v") {
            float x, y, z;
            fileStream >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        } else if (word == "f") {
            unsigned int i1, i2, i3;
            fileStream >> i1 >> i2 >> i3;
            indices.push_back(i1 - 1);
            indices.push_back(i2 - 1);
            indices.push_back(i3 - 1);
        }
    }
    initBuffers(vertices, indices);
}

void GraphicsData::initBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
//    mVertexBuffer = new VertexBuffer(mVertices.data(), mVertices.size() * sizeof(float));
    std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->allocateBuffer(vertices.size()/3,vertices);

    mVertexArray = std::make_shared<VertexArray>();
    mVertexArray->addBufferWithDefaultLayout(vertexBuffer, *VertexBufferLayout::coordinate3DOnlyLayout());

    mIndexBuffer = std::make_shared<IndexBuffer>();
    mIndexBuffer->allocateBuffer(indices);
}

GraphicsData::GraphicsData(const std::string &fileName) {
    loadFromFile(fileName);
}
GraphicsData::GraphicsData(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<IndexBuffer> indexBuffer): mVertexArray(vertexArray), mIndexBuffer(indexBuffer){
}

GraphicsData::GraphicsData(const std::vector<float> &vertices) {
    //TODO implement this
    throw "not implemented";
}

GraphicsData::GraphicsData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices){
    initBuffers(vertices, indices);
}

//GraphicsData::GraphicsData(const RigidBody &model) {
//    std::vector<float> vertices;
//    std::vector<unsigned int> indices;
//    for (int i = 0; i < model.pointMasses().size(); i++) {
//        Point r = model.pointMasses()[i].r();
//        vertices.push_back(r.x);
//        vertices.push_back(r.y);
//        vertices.push_back(r.z);
//        for (int j = i + 1; j < model.pointMasses().size(); j++) {
//            Point r2 = model.pointMasses()[j].r();
//            indices.push_back(i);
//            indices.push_back(j);
//        }
//    }
//    initBuffers(vertices, indices);
//}

GraphicsData::~GraphicsData() {
}
void GraphicsData::bind() const{
    mVertexArray->bind();
    mIndexBuffer->bind();
}
void GraphicsData::unbind() const{
    mVertexArray->unbind();
    mIndexBuffer->unbind();
}


namespace CommonModels{
    std::shared_ptr<GraphicsData> teapotModel(){
        static auto gd = std::make_shared<GraphicsData>("resources/models/teapot.obj");
        return gd;
    }
    std::shared_ptr<GraphicsData> squareModel(){
        static auto gd = std::make_shared<GraphicsData>("resources/models/square.obj");
        return gd;
    }
    std::shared_ptr<GraphicsData> cubeModel(){
        static auto gd = std::make_shared<GraphicsData>("resources/models/cube.obj");
        return gd;
    }
    std::shared_ptr<GraphicsData> arrowModel(){
        static auto gd = std::make_shared<GraphicsData>("resources/models/arrow.obj");
        return gd;
    }
}


static unsigned int generateTexture(){
    unsigned int texture;
    glGenTextures(1, &texture);
    return texture;
}

Texture::Texture(): mBufferID(generateTexture()) {
    bind();
    // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture() {
    glDeleteTextures(1, &mBufferID);
}
void Texture::allocateTexture(const Image& image){
    bind();
    if(image.getNumColorChannels() == 1){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, image.getDataPtr());
    }
    else if(image.getNumColorChannels() == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getDataPtr());
    }
    else if(image.getNumColorChannels() == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getDataPtr());
    }
    glGenerateMipmap(GL_TEXTURE_2D);
}
void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, mBufferID);
}
void Texture::bind(unsigned int textureUnit) {
    glActiveTexture(GL_TEXTURE0+textureUnit);
    bind();
}

void Texture::unbind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
