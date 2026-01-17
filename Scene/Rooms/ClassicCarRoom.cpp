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
    Renderer::loadTexture("Assets/Rooms/classic_wall4.jpg", wallTexture);
    Renderer::loadTexture("Assets/Rooms/classic_floor.jpg", floorTexture);
    //Renderer::loadTexture("Assets/Rooms/classic_ceiling.jpg", ceilingTexture);
    //Renderer::loadTexture("Assets/Rooms/clear_glass.jpg", glassTexture);
    //Renderer::loadTexture("Assets/Rooms/tinted_brown_glass.jpg", doorGlassTexture);
    
    lightColor[0] = 1.0f; lightColor[1] = 0.9f; lightColor[2] = 0.7f; lightColor[3] = 1.0f;
    ambientColor[0] = 0.2f; ambientColor[1] = 0.15f; ambientColor[2] = 0.1f; ambientColor[3] = 1.0f;
    
    carModel = new CarModel("Assets/Models/classic_car/CarMilitariObj.obj");
    carModel->load();
}

void ClassicCarRoom::draw() {
    drawRoomBase();
}

void ClassicCarRoom::drawRoomSpecifics() {
    if (carModel) {
        glPushMatrix();
        glTranslatef(m_width / 2.0f, 0.2f, m_depth / 2.0f);
       //glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(carRotation, 0.0f, 1.0f, 0.0f);

        carModel->setYScaleMultiplier(2.3f);
        carModel->setAppropriateScale(2.0f);

        carModel->draw();
        glPopMatrix();
    }
}

void ClassicCarRoom::getPlatformColor(GLfloat color[4]) {
    color[0] = 0.4f; color[1] = 0.25f; color[2] = 0.1f; color[3] = 1.0f;
}

void ClassicCarRoom::setNeonColors() {
    neonColor1[0] = 0.96f; neonColor1[1] = 0.86f; neonColor1[2] = 0.68f; neonColor1[3] = 1.0f;
    neonColor2[0] = 1.0f; neonColor2[1] = 1.0f; neonColor2[2] = 1.0f; neonColor2[3] = 1.0f;
}