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
    Renderer::loadTexture("Assets/Rooms/sport_floor2.jpg", floorTexture);
   // Renderer::loadTexture("Assets/Rooms/sport_ceiling.jpg", ceilingTexture);
    //Renderer::loadTexture("Assets/Rooms/clear_glass.jpg", glassTexture);
    //Renderer::loadTexture("Assets/Rooms/tinted_red_glass.jpg", doorGlassTexture);

    lightColor[0] = 1.0f; lightColor[1] = 0.2f; lightColor[2] = 0.2f; lightColor[3] = 1.0f;
    ambientColor[0] = 0.2f; ambientColor[1] = 0.05f; ambientColor[2] = 0.05f; ambientColor[3] = 1.0f;

    carModel = new CarModel("Assets/Models/sports_car/Avent_sport.obj");
    carModel->load();
}

void SportCarRoom::draw() {
    drawRoomBase();
}

void SportCarRoom::drawRoomSpecifics() {
    if (carModel) {
        glPushMatrix();
        glTranslatef(m_width / 2.0f, 0.2f, m_depth / 2.0f);
        //glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

        glRotatef(carRotation, 0.0f, 1.0f, 0.0f);

        carModel->setYScaleMultiplier(2.3f);
        carModel->setAppropriateScale(2.2f);

        carModel->draw();
        glPopMatrix();
    }
}

void SportCarRoom::getPlatformColor(GLfloat color[4]) {
    color[0] = 0.15f; color[1] = 0.15f; color[2] = 0.15f; color[3] = 1.0f;
}

void SportCarRoom::setNeonColors() {
    neonColor1[0] = 1.0f; neonColor1[1] = 0.1f; neonColor1[2] = 0.1f; neonColor1[3] = 1.0f;
    neonColor2[0] = 1.0f; neonColor2[1] = 1.0f; neonColor2[2] = 1.0f; neonColor2[3] = 1.0f;
}