#include "FamilyCarRoom.h"
#include <iostream>

FamilyCarRoom::FamilyCarRoom(const Vector3& position, float width, float depth, float height)
    : Room(position, width, depth, height), carModel(nullptr) {
}

FamilyCarRoom::~FamilyCarRoom() {
    if (carModel) {
        delete carModel;
    }
}

void FamilyCarRoom::loadAssets() {
    Renderer::loadTexture("Assets/Rooms/family_wall.jpg", wallTexture);
    Renderer::loadTexture("Assets/Rooms/family_floor.jpg", floorTexture);
    //Renderer::loadTexture("Assets/Rooms/family_ceiling.jpg", ceilingTexture);
   // Renderer::loadTexture("Assets/Rooms/clear_glass.jpg", glassTexture);
    //Renderer::loadTexture("Assets/Rooms/tinted_blue_glass.jpg", doorGlassTexture);

    lightColor[0] = 0.9f; lightColor[1] = 0.9f; lightColor[2] = 1.0f; lightColor[3] = 1.0f;
    ambientColor[0] = 0.15f; ambientColor[1] = 0.15f; ambientColor[2] = 0.2f; ambientColor[3] = 1.0f;

    carModel = new CarModel("Assets/Models/sports_car/1.obj");
    carModel->load();
}

void FamilyCarRoom::draw() {
    drawRoomBase();
}

void FamilyCarRoom::drawRoomSpecifics() {
    if (carModel) {
        glPushMatrix();
        glTranslatef(m_width / 2, 0.2f, m_depth / 2);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        carModel->draw();
        glPopMatrix();
    }
}

void FamilyCarRoom::getPlatformColor(GLfloat color[4]) {
    color[0] = 0.6f; color[1] = 0.4f; color[2] = 0.2f; color[3] = 1.0f;
}