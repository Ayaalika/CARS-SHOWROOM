#include "SUVCarRoom.h"
#include <iostream>

SUVCarRoom::SUVCarRoom(const Vector3& position, float width, float depth, float height)
    : Room(position, width, depth, height), carModel(nullptr) {
}

SUVCarRoom::~SUVCarRoom() {
    if (carModel) {
        delete carModel;
    }
}

void SUVCarRoom::loadAssets() {
    Renderer::loadTexture("Assets/Rooms/adventure_wall3.jpg", wallTexture);
    Renderer::loadTexture("Assets/Rooms/adventure_floor2.jpg", floorTexture);
    //Renderer::loadTexture("Assets/Rooms/adventure_ceiling.jpg", ceilingTexture);
    //Renderer::loadTexture("Assets/Rooms/clear_glass.jpg", glassTexture);
   // Renderer::loadTexture("Assets/Rooms/tinted_green_glass.jpg", doorGlassTexture);

    lightColor[0] = 0.9f; lightColor[1] = 0.9f; lightColor[2] = 0.8f; lightColor[3] = 1.0f;
    ambientColor[0] = 0.15f; ambientColor[1] = 0.15f; ambientColor[2] = 0.1f; ambientColor[3] = 1.0f;

    carModel = new CarModel("Assets/Models/adventure_suv/car_body.obj");
    carModel->load();
}

void SUVCarRoom::draw() {
    drawRoomBase();
}

void SUVCarRoom::drawRoomSpecifics() {
    if (carModel) {
        glPushMatrix();
        glTranslatef((m_width / 2.0f)+ 0.0f, 1.8f, (m_depth / 2.0f)+ 0.7f);

        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        glScalef(1.0f, 1.0f, 2.5f);
        carModel->setYScaleMultiplier(1.0f);
        carModel->setAppropriateScale(2.5f);

        carModel->draw();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }
}

void SUVCarRoom::getPlatformColor(GLfloat color[4]) {
    color[0] = 0.2f; color[1] = 0.3f; color[2] = 0.0f; color[3] = 1.0f;
}

void SUVCarRoom::setNeonColors() {
    neonColor1[0] = 0.1f; neonColor1[1] = 0.8f; neonColor1[2] = 0.2f; neonColor1[3] = 1.0f;
    neonColor2[0] = 1.0f; neonColor2[1] = 1.0f; neonColor2[2] = 1.0f; neonColor2[3] = 1.0f;
}