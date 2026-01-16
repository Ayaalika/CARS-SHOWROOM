#include <GL/freeglut.h>
#include "Bush.h"
#include "../Graphics/Renderer.h"


Bush::Bush(Vector3 position, unsigned int textureID) : m_position(position), m_textureID(textureID) {}

void Bush::draw() {
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glScalef(1.5f, 1.0f, 1.5f); 
    Renderer::drawSphere(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    Renderer::drawSphere(0.8f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    Renderer::drawSphere(0.8f);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}