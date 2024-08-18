#ifndef CAMERA_H_
#define CAMERA_H_
#include "matrices.h"

#define M_PI_4 0.78539816339

class Camera {

    public:
        glm::vec4 cartesianPosition;
        bool useFreeCamera;
        float nearPlane;
        float farPlane;
        glm::vec4 viewVector;
        glm::vec4 lookAt;
        glm::vec4 upVector;
        bool up, down, left, right, ataque;
        float phi, theta, distance;
        //FONTE: laborat�rios da disciplina
        Camera();
        glm::mat4 getView();
        void updateViewVector();
        void updateCamera(float delta_t);
        void updateViewVector(float angleX, float angleY);
        void updateSphericAngles(float dx, float dy);
        void updateSphericAngles(float angle);
        void updateSphericDistance(float distance);
    };

#endif //CAMERA_H_
