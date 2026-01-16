#include <GL/freeglut.h>
#include "Footpath.h"
#include "../Graphics/Renderer.h"

Footpath::Footpath() {}

void Footpath::draw() {
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_TEXTURE_2D);
    for (int i = 1; i < 20; i++) {
        glPushMatrix();
        glTranslatef(-15, -2.7, -10 * i);
        glScaled(7.5, 1, 10);
        Renderer::drawCube(1, 1, 1, 3);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(9, -2.7, -10 * i);
        glScaled(6, 1, 10);
        Renderer::drawCube(1, 1, 1, 3);
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
}
void Footpath::setTexture(unsigned int textureID) {
    m_textureID = textureID;
}