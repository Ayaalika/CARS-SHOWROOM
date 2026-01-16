#pragma once
class Environment {
public:
    Environment();
    void draw();
    void setTextures(unsigned int grassID, unsigned int rockID);
private:
    unsigned int m_grassTextureID;
    unsigned int m_rockTextureID;
};