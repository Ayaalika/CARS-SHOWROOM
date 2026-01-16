#pragma once
#include "../Math/Vector3.h"

class Building {
public:
    Building(const Vector3& position, float width, float height, float depth, unsigned int textureID);
    void draw();

private:
    Vector3 m_position;
    float m_width, m_height, m_depth;
    unsigned int m_textureID;
};