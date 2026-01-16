#pragma once
class Road {
public:
    Road();
    void draw();
    void setTexture(unsigned int textureID);
private:
    unsigned int m_textureID;
};