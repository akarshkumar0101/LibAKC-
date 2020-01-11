//
// Created by Akarsh Kumar on 12/11/19.
//

#include "Camera.h"


void Camera::updateVectors() {
    //mPosition and mFront are set
    mRight = glm::cross(mFront, mWorldUp);
    mUp = glm::cross(mRight, mFront);
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp) : mPosition(position), mFront(front),
                                                                         mWorldUp(worldUp),
                                                                         mMovementSpeed(DEFAULT_MOVEMENT_SPEED),
                                                                         mLookingSpeed(DEFAULT_LOOKING_SPEED),
                                                                         mFOVy(DEFAULT_FOVY) {
    updateVectors();
}

glm::mat4 Camera::computeViewMatrix() const {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::mat4 Camera::computePerspectiveProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(mFOVy), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::computePerspectiveOrthogonalMatrix() const {
    return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
}

void Camera::move(CameraMovement movement, float dt) {
    float speed = mMovementSpeed * dt;
    switch (movement) {
        case FORWARD: {
            mPosition += mFront * speed;
            break;
        }
        case BACKWARD: {
            mPosition -= mFront * speed;
            break;
        }
        case RIGHTWARD: {
            mPosition += mRight * speed;
            break;
        }
        case LEFTWARD: {
            mPosition -= mRight * speed;
            break;
        }
        case UPWARD: {
            mPosition += mUp * speed;
            break;
        }
        case DOWNWARD: {
            mPosition -= mUp * speed;
            break;
        }
    }
}

void Camera::look(float xOffset, float yOffset) {
    xOffset *= -mLookingSpeed;
    yOffset *= -mLookingSpeed;

    glm::mat4 rotationalMat(1.0);
    rotationalMat = glm::rotate(rotationalMat, xOffset, mUp);
    rotationalMat = glm::rotate(rotationalMat, yOffset, mRight);

    mFront = rotationalMat * glm::vec4(mFront, 1.0);

    updateVectors();
}