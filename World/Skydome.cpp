#include <GL/freeglut.h>
#include "Skydome.h"
#include <GL/glu.h>

Skydome::Skydome(unsigned int textureID) : m_textureID(textureID) {}

void Skydome::draw(const Vector3& cameraPosition) {
    if (m_textureID == 0) return; 

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluSphere(quadric, 200.0f, 32, 32); 
    glPopMatrix();

    gluDeleteQuadric(quadric);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}