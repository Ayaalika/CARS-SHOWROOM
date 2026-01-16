#pragma once

class Texture {
public:
    Texture();
    ~Texture();
    bool Load(const char* filePath);
    void Bind() const;
    unsigned int GetID() const;

private:
    unsigned int m_textureID;
    int m_width, m_height, m_channels;
};