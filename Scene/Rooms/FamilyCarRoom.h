#pragma once

#include "../Room.h"
#include "../CarModel.h"

class FamilyCarRoom : public Room {
public:
    FamilyCarRoom(const Vector3& position, float width, float depth, float height);
    virtual ~FamilyCarRoom();

    virtual void loadAssets() override;
    virtual void draw() override;

protected:
    virtual void drawRoomSpecifics() override;
    virtual void getPlatformColor(GLfloat color[4]);

private:
    CarModel* carModel;
};