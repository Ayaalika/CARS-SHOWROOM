#pragma once
#include "../Room.h"

#include "../CarModel.h"

class SportCarRoom : public Room {
public:
    SportCarRoom(const Vector3& position, float width, float depth, float height);
    ~SportCarRoom();

    void loadAssets() override;
    void draw() override;

protected:
    void drawRoomSpecifics() override;
    void setNeonColors() override;
    void getPlatformColor(GLfloat color[4]) override;

private:
    CarModel* carModel;
    float carScale;
    float carRotationSpeed;
};