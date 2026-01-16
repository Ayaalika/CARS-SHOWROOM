#include "ClassicCarRoom.h"
#include <iostream>

ClassicCarRoom::ClassicCarRoom(const Vector3& position, float width, float depth, float height)
    : Room(position, width, depth, height), carModel(nullptr) {
}

ClassicCarRoom::~ClassicCarRoom() {
    if (carModel) {
        delete carModel;
    }
}

void ClassicCarRoom::loadAssets() {
    Renderer::loadTexture("Assets/Rooms/classic_wall.jpg", wallTexture);
    Renderer::loadTexture("Assets/Rooms/classic_floor.jpg", floorTexture);
    //Renderer::loadTexture("Assets/Rooms/classic_ceiling.jpg", ceilingTexture);
    //Renderer::loadTexture("Assets/Rooms/clear_glass.jpg", glassTexture);
    //Renderer::loadTexture("Assets/Rooms/tinted_brown_glass.jpg", doorGlassTexture);
    
    lightColor[0] = 1.0f; lightColor[1] = 0.9f; lightColor[2] = 0.7f; lightColor[3] = 1.0f;
    ambientColor[0] = 0.2f; ambientColor[1] = 0.15f; ambientColor[2] = 0.1f; ambientColor[3] = 1.0f;
    
    carModel = new CarModel("Assets/Models/sports_car/1.obj");
    carModel->load();
}

void ClassicCarRoom::draw() {
    drawRoomBase();
}

void ClassicCarRoom::drawRoomSpecifics() {
    if (carModel) {
        glPushMatrix();
        glTranslatef(m_width / 2, 0.2f, m_depth / 2);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        carModel->draw();
        glPopMatrix();
    }
}

void ClassicCarRoom::getPlatformColor(GLfloat color[4]) {
    color[0] = 0.7f; color[1] = 0.5f; color[2] = 0.3f; color[3] = 1.0f;
}