#include "../include/camera.h"

Camera::Camera() {
    this->cartesianPosition = glm::vec4(0.4f, 0.8f, 0.0f, 1.0f);
    this->useFreeCamera = false;
    this->lookAt = glm::vec4(0.5f, 0.8f, 0.0f, 1.0f);
    this->viewVector = normalize(this->lookAt - this->cartesianPosition);
    this->upVector = glm::vec4(0.0f,1.0f,0.0f,0.0f);
    this->nearPlane = -0.1f;
    this->farPlane = -50.0f;
    this->up = false;
    this->down = false;
    this->left = false;
    this->right = false;
    this->ataque = false;
    this->phi = 0.0f;
    this->theta = 0.0f;
    this->distance = 1.0f;
}

void Camera::updateViewVector() {
    glm::vec4 vec = lookAt;
    vec.x += this->distance * cos(this->phi) * sin(this->theta);
    vec.y += this->distance * sin(this->phi);
    vec.z += this->distance * cos(this->phi) * cos(this->theta);
    vec.w = 1.0f;
    this->viewVector = normalize(this->lookAt - vec);
}

void Camera::updateViewVector(float angleX, float angleY) {
    if (angleX == 0 && angleY == 0)
        return;
    else {
        glm::vec4 side = crossproduct(Camera::upVector, this->viewVector);
        glm::vec4 aux = this->viewVector * Matrix_Rotate(-angleY, side);
        if(dotproduct(side, crossproduct(Camera::upVector, aux)) > 0)
            this->viewVector = aux;
        this->viewVector = normalize(this->viewVector * Matrix_Rotate(angleX, Camera::upVector));
    }
}

void Camera::updateSphericAngles(float dx, float dy) {
    float newPhi = this->phi + 0.003f * dy;
    if (newPhi > M_PI / 4)
        newPhi = M_PI / 4;
    if (newPhi < 0)
        newPhi = 0;
    this->phi = newPhi;
    this->theta -= 0.003f * dx;
}

void Camera::updateSphericAngles(float angle) {
    this->theta -= angle;
}

void Camera::updateSphericDistance(float distance) {
    this->distance -= 0.2f * distance;
    if (this->useFreeCamera)
        this->useFreeCamera = false;
    if (this->distance < 0.5f) {
        this->distance = 0.5f;
        this->useFreeCamera = true;
    }
}

glm::mat4 Camera::getView() {
    if (this->useFreeCamera)
        return Matrix_Camera_View(this->cartesianPosition, this->viewVector, Camera::upVector);
    else {
        glm::vec4 pos;
        pos.x = this->distance * cos(this->phi) * sin(this->theta);
        pos.y = this->distance * sin(this->phi);
        pos.z = this->distance * cos(this->phi) * cos(this->theta);
        pos.w = 1.0f;
        glm::vec4 position = lookAt + pos;
        position.w = 1.0f;
        return Matrix_Camera_View(position, this->viewVector, Camera::upVector);
    }
}

void Camera::updateCamera(float delta_t) {
    float cameraSpeed = 2.0f;
    glm::vec4 w = -(normalize(this->viewVector));
    glm::vec4 u = normalize(crossproduct(Camera::upVector, w));
    if (this->up)
        this->cartesianPosition -= w * cameraSpeed * delta_t;
    if (this->down)
        this->cartesianPosition += w * cameraSpeed * delta_t;
    if (this->left)
        this->cartesianPosition -= u * cameraSpeed * delta_t;
    if (this->right)
        this->cartesianPosition += u * cameraSpeed * delta_t;
    this->cartesianPosition.y = 0.8f;
    if (!this->useFreeCamera)
        this->updateViewVector();
}
