//
// Created by Akarsh Kumar on 12/10/19.
//
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>



void Shader::loadShadersSource() {
    std::ifstream fileStream(mShaderSourceFilePath);

    std::stringstream sources[2];

    enum ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::stringstream &vs = sources[VERTEX], &fs = sources[FRAGMENT];

    ShaderType currentType = NONE;
    std::string line;
    while (getline(fileStream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                currentType = VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                currentType = FRAGMENT;
            }
        } else {
            sources[currentType] << line << std::endl;
        }
    }
    mSps = {vs.str(), fs.str()};
}

Shader::Shader(const std::string &shaderSourceFilePath) : mVertexShader(glCreateShader(GL_VERTEX_SHADER)),
                                                  mFragmentShader(glCreateShader(GL_FRAGMENT_SHADER)),
                                                  mShaderProgramID(glCreateProgram()),
                                                  mShaderSourceFilePath(shaderSourceFilePath) {
    this->loadShadersSource();

    const char *vsSource = mSps.vertexShader.c_str();
    const char *fsSource = mSps.fragmentShader.c_str();

    glShaderSource(mVertexShader, 1, &vsSource, nullptr);
    glShaderSource(mFragmentShader, 1, &fsSource, nullptr);

    glCompileShader(mVertexShader);
    glCompileShader(mFragmentShader);

    glAttachShader(mShaderProgramID, mVertexShader);
    glAttachShader(mShaderProgramID, mFragmentShader);
    glLinkProgram(mShaderProgramID);

    checkCompilationLinkingIssues();

    glDeleteShader(mVertexShader);
    glDeleteShader(mFragmentShader);
}

void Shader::checkCompilationLinkingIssues() const{
    using namespace std;

    int successVS, successFS, successProgram;
    char infoLog[512];
    glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &successVS);
    glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &successFS);
    glGetProgramiv(mShaderProgramID, GL_LINK_STATUS, &successProgram);
    if (!successVS) {
        glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);
        cerr << "Vertex shader compilation error: " << endl;
        cerr << infoLog << endl;
    }
    if (!successFS) {
        glGetShaderInfoLog(mFragmentShader, 512, nullptr, infoLog);
        cerr << "Fragment shader compilation error: " << endl;
        cerr << infoLog << endl;
    }
    if (!successProgram) {
        glGetProgramInfoLog(mShaderProgramID, 512, nullptr, infoLog);
        cerr << "Shader Program linking error: " << endl;
        cerr << infoLog << endl;
    }
}

void Shader::bind() const {
    glUseProgram(mShaderProgramID);
}
void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setUniform(const std::string& name, const float val) const {
    bind();
    glUniform1f(getUniformLocation((name)), val);
}
void Shader::setUniform(const std::string& name, const glm::mat4& mat) const {
    bind();
    glUniformMatrix4fv(getUniformLocation(name),1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setUniform(const std::string& name, const glm::vec4& vec) const {
    bind();
    glUniform4fv(getUniformLocation(name),1,glm::value_ptr(vec));
}

int Shader::getUniformLocation(const std::string& name) const {
    int location = glGetUniformLocation(mShaderProgramID, name.c_str());
    if(location==-1){
        throw std::string("Invalid name for uniform");
    }
    return location;
}
int Shader::getAttribLocation(const std::string name) const{
    int location = glGetAttribLocation(mShaderProgramID, name.c_str());
    if(location==-1){
        throw std::string("Invalid name for attribute");
    }
    return location;
}
