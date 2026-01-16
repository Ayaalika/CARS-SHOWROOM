#pragma once
#include "../Math/Vector3.h"

class Arch {
public:
    Arch(Vector3 position, unsigned int textureID);
    void draw();

private:
    Vector3 m_position;
    unsigned int m_textureID;
};