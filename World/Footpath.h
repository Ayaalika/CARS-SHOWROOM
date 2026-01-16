#pragma once
class Footpath {
public:
    Footpath();
    void draw();
    void setTexture(unsigned int textureID);
private:
    unsigned int m_textureID;
};