#include <GL/freeglut.h>
#include "Environment.h"
#include "../Graphics/Renderer.h"

Environment::Environment() {}

void Environment::draw() {
    for (int i = 1; i < 20; i++) {
            glBindTexture(GL_TEXTURE_2D, m_grassTextureID);
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glTranslatef(-100, -3, i * -10);
            glScaled(200, 1, 110);
            Renderer::drawCube(1.0f, 1.0f, 1.0f, 30); 
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, m_rockTextureID);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-100, -3.5, 21 * -10);
    glScaled(200, 1, 110); 
    Renderer::drawCube(1.0f, 1.0f, 1.0f, 30);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Environment::setTextures(unsigned int grassID, unsigned int rockID) {
    m_grassTextureID = grassID;
    m_rockTextureID = rockID;
}