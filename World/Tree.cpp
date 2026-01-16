#include <GL/freeglut.h>
#include "Tree.h"
#include "../Graphics/Renderer.h"

Tree::Tree(const Vector3& position) : m_position(position) {
    m_height = (rand() % 5) + 8.0f;
}

void Tree::draw() {
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glBindTexture(GL_TEXTURE_2D, m_barkTextureID);
    glEnable(GL_TEXTURE_2D);
    Renderer::drawCylinder(0.4, 0.3, m_height);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y + m_height + 1.0f, m_position.z);
    glBindTexture(GL_TEXTURE_2D, m_leavesTextureID);
    glEnable(GL_TEXTURE_2D);
    Renderer::drawSphere(2.3f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Tree::setTextures(unsigned int barkID, unsigned int leavesID) {
    m_barkTextureID = barkID;
    m_leavesTextureID = leavesID;
}