#include "Camera.h"
#include <GL/freeglut.h>
#include <cmath>

const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;

Camera::Camera(Vector3 position, Vector3 up, float yaw, float pitch) : Front(Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

void Camera::applyViewMatrix() {
    Vector3 target = Position + Front;
    gluLookAt(Position.x, Position.y, Position.z,
        target.x, target.y, target.z,
        Up.x, Up.y, Up.z);
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == 0) 
        Position = Position + Front * velocity;
    if (direction == 1) 
        Position = Position - Front * velocity;
    if (direction == 2)
        Position = Position - Right * velocity;
    if (direction == 3)
        Position = Position + Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    Vector3 front;
    front.x = cos(Vector3::radians(Yaw)) * cos(Vector3::radians(Pitch));
    front.y = sin(Vector3::radians(Pitch));
    front.z = sin(Vector3::radians(Yaw)) * cos(Vector3::radians(Pitch));
    Front = front;
    Front.normalize();

    Right = Front.cross(WorldUp);
    Right.normalize();
    Up = Right.cross(Front);
    Up.normalize();
}