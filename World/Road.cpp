#include <GL/freeglut.h>
#include "Road.h"
#include "../Graphics/Renderer.h"

Road::Road() {}

void Road::draw() {
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_TEXTURE_2D);
    for (int i = 1; i < 20; i++) {
        if (i <= 19 || i >= 22) {
            glPushMatrix();
            glTranslatef(-7.5, -2.9, -10 * i);
            glScaled(17, 1, 10);
            Renderer::drawCube(1, 1, 1, 1);
            glPopMatrix();
        }
    }
    glDisable(GL_TEXTURE_2D);
}
void Road::setTexture(unsigned int textureID) {
    m_textureID = textureID;
}