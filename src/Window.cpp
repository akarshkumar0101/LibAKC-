//
// Created by Akarsh Kumar on 12/11/19.
//

#include <iostream>

#include "Window.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"



static void errorCallback(int error, const char* description){
    std::cerr<<"GLFW Error "<<error<<": "<<description<<std::endl;
}

void Window::init() {
    glfwSetErrorCallback(errorCallback);
    glfwInit();
}

Window::Window(const int width, const int height, const std::string& title): Window(width, height, title, nullptr, nullptr){
}
Window::Window(const int width, const int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share){
    init();

    //const char* glsl_version = "#version 150";


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

    mWindow = glfwCreateWindow(width,height,title.c_str(),nullptr,nullptr);
    if(mWindow==nullptr){
        std::cout<<"failed to create window"<<std::endl;
        glfwTerminate();
        throw "Window not created";
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1); // Enable vsync

    int gladLoadSuccess = gladLoadGL();

    if(gladLoadSuccess==0){
        std::cout<<"failed to load with glad"<<std::endl;
        glfwTerminate();
        throw "Could not initialize glad";
    }

    std::cout<< "GL Version: "<<glGetString(GL_VERSION)<<std::endl;
    std::cout<< "GL Shading Language Version: " <<glGetString ( GL_SHADING_LANGUAGE_VERSION )<<std::endl;

}
Window::~Window(){
    glfwTerminate();
}

void Window::getFramebufferSize(int &width, int &height) {
    glfwGetFramebufferSize(mWindow, &width, &height);
}

void Window::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(mWindow, shouldClose);
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(mWindow);
}

GLFWwindow *Window::getRawWindowPointer() {
    return mWindow;
}

void Window::makeContextCurrent() {
    glfwMakeContextCurrent(mWindow);
}

void Window::swapBuffers() {
    glfwSwapBuffers(mWindow);
}

bool Window::isKeyReleased(int key) {
    return glfwGetKey(mWindow, key) == GLFW_RELEASE;
}

bool Window::isKeyPressed(int key) {
    return glfwGetKey(mWindow, key) == GLFW_PRESS;
}

bool Window::isKeyRepeated(int key) {
    return glfwGetKey(mWindow, key) == GLFW_RELEASE;
}

void Window::getCursorPosition(double &xPos, double& yPos) {
    glfwGetCursorPos(mWindow, &xPos, &yPos);
}


void Window::setRoot(std::shared_ptr<Pane> root){
    mRoot = root;
}
void Window::render(){
    if(mRoot){
        int width, height;
        getFramebufferSize(width, height);
        mRoot->render(Viewport(0,0,width, height));
    }
}

void Window::pollEvents() {
    glfwPollEvents();
}

bool Window::cursorIsInWindow() {
    //temporary solution:
    double x, y;
    getCursorPosition(x, y);
    int width, height;
    getFramebufferSize(width, height);
    return x>=0 && y>=0 && x<=width && y <=height;
}



