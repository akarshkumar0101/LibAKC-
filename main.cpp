#include <iostream>
#include "src/graphics/TrianglePane.h"
#include "src/graphics/GridPane.h"
#include "src/graphics/DrawingPane.h"
#include "src/graphics/ImagePane.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glfw/glfw3.h>


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

void test2Windows(){
    Window window1(300,300,"window1");
    Window window2(300,300,"window2");

    while(!window1.shouldClose()){
        window1.swapBuffers();
        window1.pollEvents();

        window2.swapBuffers();
        window2.pollEvents();
    }
}

void testError(){
    auto window = std::make_shared<Window>(500,500,"hey");

    Image* imgs[3];
    imgs[0] = new Image("/Users/akarshkumar0101/Pictures/Wallpapers/space galxy with man.jpg");
    imgs[1] = new Image("/Users/akarshkumar0101/Pictures/Wallpapers/daylight-environment-forest-idyllic-459225.jpg");
//    imgs[2] = new Image("resources/img/awesomeface.png");

//    Image img(mnist::imageWidth,mnist::imageHeight,1, mnist::testImages->data()+(28*28*100));
    auto gridPane = std::make_shared<GridPane>(window, std::pair{2,1});

    std::vector<std::shared_ptr<ImagePane>> imagePanes(2);
    imagePanes[0] = std::make_shared<ImagePane>(window);
    gridPane->setChild(imagePanes[0],{0,0});

//    gridPane->setChild(imagePanes[1],{1,0});

//    imagePanes[1] = std::make_shared<ImagePane>(window);
//    Texture tex;
    unsigned int temp;
    glGenTextures(1, &temp);
    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, temp);
//    glActiveTexture(GL_TEXTURE0);


    imagePanes[0]->setImage(*imgs[0]);




    window->setRoot(gridPane);

    int batch=-1;
    bool isPressed = false;

    while(!window->shouldClose()){
        if(window->isKeyPressed(GLFW_KEY_SPACE) && !isPressed){
            isPressed = true;
            batch++;

//            imagePanes[0]->setImage(*imgs[batch%3]);
            std::cout<<"press"<<std::endl;
        }
        if(!(window->isKeyPressed(GLFW_KEY_SPACE))){
            isPressed = false;
        }

        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        window->render();

        window->swapBuffers();
        window->pollEvents();
    }

}
int main() {
//    testError();
//    test2Windows();
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