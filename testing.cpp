//
// Created by Akarsh Kumar on 1/6/20.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "Window.h"
#include "GraphicsBuffer.h"


int main()
{
    Window window(900,900,"doggy");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout vbl;
    vbl.addElement<float>(3,"vPos");

    Shader shader("./resources/shaders/triangle.shader");

    // render loop
    // -----------
    while (!window.shouldClose())
    {

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        //glUseProgram(shaderProgram);
        shader.bind();

        vb.bind();
//        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


        vbl.bindVertexAttributesToShader(shader);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}
