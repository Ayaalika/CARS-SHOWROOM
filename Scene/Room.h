#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <GL/freeglut.h> 
#include "../tiny_obj_loader.h"
#include "../Math/Vector3.h" 
#include "../Graphics/Renderer.h"

class Room {
public:
    Room(const Vector3& position, float width, float depth, float height);
    virtual ~Room();

    virtual void loadAssets() = 0;
    virtual void draw() = 0;
    virtual void updateDoorState(const Vector3& cameraPosition, float scaleFactor, const Vector3& showroomOrigin);

    Vector3 getPosition() const { return m_position; }
    float getWidth() const { return m_width; }
    float getDepth() const { return m_depth; }
    float getHeight() const { return m_height; }

    virtual void update(float deltaTime);
    float getRotationSpeed() const { return rotationSpeed; }
    void setRotationSpeed(float speed) { rotationSpeed = speed; }
protected:
    Vector3 m_position;
    float m_width;
    float m_depth;
    float m_height;

    float doorPosition;
    bool doorOpening;
    bool doorClosing;

    GLuint wallTexture;
    GLuint floorTexture;
    GLuint ceilingTexture;
    GLuint glassTexture;
    GLuint doorGlassTexture;

    GLfloat lightColor[4];
    GLfloat ambientColor[4];

    void drawRoomBase();
    void drawGlassFacade(bool isLeftRoom);
    void drawPlatform();
    virtual void drawRoomSpecifics() = 0;

    virtual void getPlatformColor(GLfloat color[4]) = 0;

    float carRotation;   
    float rotationSpeed;

    GLfloat neonColor1[4]; 
    GLfloat neonColor2[4];

    void drawNeonCeiling();
    virtual void setNeonColors() {};
private:
    static const float DOOR_OPEN_DISTANCE;
    static const float DOOR_SPEED;
};