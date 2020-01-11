//
// Created by Akarsh Kumar on 11/22/19.
//

#pragma once

#include <string>
#include <glm/glm.hpp>


struct ShaderProgramSource {
    std::string vertexShader;
    std::string fragmentShader;
};


class Shader {
private:
    const unsigned int mVertexShader;
    const unsigned int mFragmentShader;
    const unsigned int mShaderProgramID;

    const std::string mShaderSourceFilePath;

    ShaderProgramSource mSps;

    void loadShadersSource();

public:
    Shader(const std::string &shaderSourceFilePath);

    void checkCompilationLinkingIssues() const;

    void bind() const;
    void unbind() const;

    void setUniform(const std::string& name, const float val) const;
    void setUniform(const std::string& name, const glm::mat4& mat) const;
    void setUniform(const std::string& name, const glm::vec4& vec) const;

    int getUniformLocation(const std::string& name) const;
    int getAttribLocation(const std::string name) const;

};