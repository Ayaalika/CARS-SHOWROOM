#pragma once

#include "../Room.h"
#include "../CarModel.h"

class ClassicCarRoom : public Room {
public:
    ClassicCarRoom(const Vector3& position, float width, float depth, float height);
    virtual ~ClassicCarRoom();

    virtual void loadAssets() override;
    virtual void draw() override;

protected:
    virtual void drawRoomSpecifics() override;
    void setNeonColors() override;
    virtual void getPlatformColor(GLfloat color[4]);

private:
    CarModel* carModel;
};