#pragma once

#include "../Room.h"
#include "../CarModel.h"

class SUVCarRoom : public Room {
public:
    SUVCarRoom(const Vector3& position, float width, float depth, float height);
    virtual ~SUVCarRoom();

    virtual void loadAssets() override;
    virtual void draw() override;

protected:
    virtual void drawRoomSpecifics() override;
    virtual void getPlatformColor(GLfloat color[4]);

private:
    CarModel* carModel;
};