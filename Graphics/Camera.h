#pragma once
#include "../Math/Vector3.h"
#include <GL/freeglut.h>
class Camera {
public:
    Vector3 Position;
    Vector3 Front;
    Vector3 Up;
    Vector3 Right;
    Vector3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;

    Camera(Vector3 position = Vector3(0.0f, 1.0f, -150.0f), Vector3 up = Vector3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

    void applyViewMatrix();

    void ProcessKeyboard(int direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
private:
    void updateCameraVectors();
};