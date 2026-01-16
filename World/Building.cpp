#include <GL/freeglut.h>
#include "Building.h"
#include "../Graphics/Renderer.h"

Building::Building(const Vector3& position, float width, float height, float depth, unsigned int textureID)
    : m_position(position), m_width(width), m_height(height), m_depth(depth), m_textureID(textureID) {}

void Building::draw() {
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_width, m_height, m_depth);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_TEXTURE_2D);
    Renderer::drawCube(1, 1, 1, 1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}