#pragma once
#include "../Math/Vector3.h"

class Skydome {
public:
    Skydome(unsigned int textureID);
    void draw(const Vector3& cameraPosition);

private:
    unsigned int m_textureID;
};