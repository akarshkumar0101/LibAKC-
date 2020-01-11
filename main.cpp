#include <iostream>

#include "include/TrianglePane.h"
#include "include/GridPane.h"
#include "include/DrawingPane.h"
#include "include/ImagePane.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void testLines(){
    Window window(900,900, "hey window");

    Shader shader("./resources/shaders/triangle.shader");

    std::vector<float> vertices({0,0,0});
    std::vector<unsigned int> indices;
    for(float deg=0;deg<2*M_PI;deg+=0.2){
        vertices.insert(vertices.end(),{(float) cos(deg),(float) sin(deg),0.0f});
    }
    for(unsigned int i=1; i <vertices.size();i++){
        indices.insert(indices.end(), {0, i});
    }


    GraphicsData gd(vertices, indices);

    glClearColor(0.0,0.0,0.0,1.0);
    while(!window.shouldClose()){
        glClear(GL_COLOR_BUFFER_BIT);
        shader.bind();

        gd.bind();

        glDrawElements(GL_LINES, gd.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);

        window.swapBuffers();
        window.pollEvents();
    }
}

void renderFunction(Graphics& graphics){
    graphics.setColor(Color::GRAY);
    graphics.clear();

    graphics.setColor(Color(1.0,0.0,0.0,1.0));
    graphics.drawPoint(0,0);
    graphics.setColor(Color(1.0,1.0,0.0,1.0));
    graphics.drawLine(-0.5,0,0.5,0.0);
    graphics.setColor(Color(1.0,0.0,1.0,1.0));
    graphics.drawLine(0,-0.5,0.0,0.5);
    graphics.setColor(Color(0.0,0.0,1.0,1.0));
    graphics.fillTriangle(0.5,0,0,-0.5,1,-0.5);
    graphics.setColor(Color(0.0,1.0,0.0,1.0));
    graphics.fillCircle(0.5,0.5,0.5);


}

void testWindowAndNode(){
    auto window = std::make_shared<Window>(900,900, "hey window");

    auto gridNode = std::make_shared<GridPane> (window, std::pair<unsigned int, unsigned int>({3, 3}));
    auto gridNode2 = std::make_shared<GridPane> (window, std::pair<unsigned int, unsigned int>({3, 3}));

    auto drawingNode = std::make_shared<DrawingPane> (window, renderFunction);
    auto drawingNode2 = std::make_shared<DrawingPane> (window, renderFunction);

    auto image = std::make_shared<Image>("/Users/akarshkumar0101/Pictures/Wallpapers/space galxy with man.jpg");
    auto image1 = std::make_shared<Image>("/Users/akarshkumar0101/Pictures/Wallpapers/daylight-environment-forest-idyllic-459225.jpg");
    auto image2 = std::make_shared<Image>("resources/img/awesomeface.png");

    auto imageNode = std::make_shared<ImagePane> (window, *image);
    auto imageNode2 = std::make_shared<ImagePane> (window, *image2);


    window->setRoot(gridNode);

    for(int x=0;x<3; x++){
        for(int y=0; y<3; y++){
            auto triangleNode = std::make_shared<TrianglePane>(window);
            gridNode->setChild(triangleNode, {x,y});
        }
    }
    gridNode->setChild(gridNode2, {1,1});
    gridNode->setChild(drawingNode, {0,0});
    gridNode->setChild(imageNode, {2, 2});
    for(int x=0;x<3; x++){
        for(int y=0; y<3; y++){
            auto triangleNode = std::make_shared<TrianglePane>(window);
            gridNode2->setChild(triangleNode, {x,y});
        }
    }
    gridNode2->setChild(drawingNode2, {1,0});
    gridNode2->setChild(imageNode, {0, 2});
    gridNode2->setChild(imageNode2, {1, 2});

    glClearColor(0.0,0.0,0.0,1.0);

    int fps = 0;
    double pt = glfwGetTime();
    int sec = (int) pt;
    while(!window->shouldClose()){
        double t = glfwGetTime();
        if((int)t != sec){
            sec = (int)t;
//            std::cout<<fps<<std::endl;
            if(sec%2==0){
                imageNode->setImage(*image1);
            }
            else{
                imageNode->setImage(*image);
            }
            fps = 0;
        }
        fps++;
        pt = t;

        glClear(GL_COLOR_BUFFER_BIT);

        window->render();

        window->swapBuffers();
        window->pollEvents();
    }

}
int main() {
    testWindowAndNode();
//    testingOpenGL();
//    try{
//        simulation();
//    }catch(std::string exp){
//        std::cout<<exp<<std::endl;
//    }

//    Window window(900,900, "dog window");
//
//    while(!window.shouldClose()){
//
//        window.swapBuffers();
//        window.pollEvents();
//    }
    return 0;
}