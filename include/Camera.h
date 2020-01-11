//
// Created by Akarsh Kumar on 11/27/19.
//

#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


enum CameraMovement {
    FORWARD, BACKWARD, LEFTWARD, RIGHTWARD, UPWARD, DOWNWARD
};

class Camera {
public:
    static constexpr float DEFAULT_MOVEMENT_SPEED = 1.0f;
    static constexpr float DEFAULT_LOOKING_SPEED = 0.01f;
    static constexpr float DEFAULT_FOVY = 45.0f;
private:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;

    glm::vec3 mWorldUp;


    float mMovementSpeed;
    float mLookingSpeed;
    float mFOVy;

    void updateVectors();
public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp);

    glm::mat4 computeViewMatrix() const;
    glm::mat4 computePerspectiveProjectionMatrix(float aspectRatio) const ;
    glm::mat4 computePerspectiveOrthogonalMatrix() const ;

    void move(CameraMovement movement, float dt) ;

    void look(float xOffset, float yOffset) ;

};