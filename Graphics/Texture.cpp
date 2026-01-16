#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#include "Texture.h"
#include <GL/freeglut.h>
#include <iostream>

Texture::Texture() : m_textureID(0), m_width(0), m_height(0), m_channels(0)
{
}

Texture::~Texture() { 
}

bool Texture::Load(const char* filePath)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(
        filePath,
        &m_width,
        &m_height,
        &m_channels,
        0
    );

    if (!data)
    {
        std::cerr << "ERROR: Failed to load texture: " << filePath << std::endl;
        return false;
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB;
    if (m_channels == 4)
        format = GL_RGBA;

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        m_width,
        m_height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);

    return true;
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

unsigned int Texture::GetID() const
{
    return m_textureID;
}