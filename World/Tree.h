#pragma once
#include "../Math/Vector3.h"

class Tree {
public:
    Tree(const Vector3& position);
    void draw();
    void setTextures(unsigned int barkID, unsigned int leavesID);
private:
    Vector3 m_position;
    float m_height;
    unsigned int m_barkTextureID;
    unsigned int m_leavesTextureID;
};