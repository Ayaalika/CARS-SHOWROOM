#include "SportCarRoom.h"
#include <iostream>

SportCarRoom::SportCarRoom(const Vector3& position, float width, float depth, float height)
    : Room(position, width, depth, height), carModel(nullptr), carScale(1.0f), carRotationSpeed(0.0f) {
}

SportCarRoom::~SportCarRoom() {
    if (carModel) {
        delete carModel;
    }
}

void SportCarRoom::loadAssets() {
    Renderer::loadTexture("Assets/Rooms/sport_wall.jpg", wallTexture);
    Renderer::loadTexture("Assets/Rooms/sport_floor.jpg", floorTexture);
   // Renderer::loadTexture("Assets/Rooms/sport_ceiling.jpg", ceilingTexture);
    //Renderer::loadTexture("Assets/Rooms/clear_glass.jpg", glassTexture);
    //Renderer::loadTexture("Assets/Rooms/tinted_red_glass.jpg", doorGlassTexture);

    lightColor[0] = 1.0f; lightColor[1] = 0.2f; lightColor[2] = 0.2f; lightColor[3] = 1.0f;
    ambientColor[0] = 0.2f; ambientColor[1] = 0.05f; ambientColor[2] = 0.05f; ambientColor[3] = 1.0f;

    carModel = new CarModel("Assets/Models/sports_car/1.obj");
    carModel->load();
}

void SportCarRoom::draw() {
    drawRoomBase();
}

void SportCarRoom::drawRoomSpecifics() {
    if (carModel) {
        glPushMatrix();
        glTranslatef(m_width / 2.0f, 0.2f, m_depth / 2.0f);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);

        carModel->setAppropriateScale(2.0f);

        carModel->draw();
        glPopMatrix();
    }
}

void SportCarRoom::getPlatformColor(GLfloat color[4]) {
    color[0] = 0.15f; color[1] = 0.15f; color[2] = 0.15f; color[3] = 1.0f;
}