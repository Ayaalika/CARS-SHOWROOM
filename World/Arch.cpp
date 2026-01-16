#include <GL/freeglut.h>
#include "Arch.h"
#include "../Graphics/Renderer.h"
#include <string>

Arch::Arch(Vector3 position, unsigned int textureID) : m_position(position), m_textureID(textureID) {}

void Arch::draw() {
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);

    glPushMatrix();
    glTranslatef(5, 0, 0);
    glScalef(1, 8, 1);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_TEXTURE_2D);
    Renderer::drawCube(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5, 0, 0);
    glScalef(1, 8, 1);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_TEXTURE_2D);
    Renderer::drawCube(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 8, 0);
    glScalef(12, 1, 1);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glEnable(GL_TEXTURE_2D);
    Renderer::drawCube(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(1.0f, 1.0f, 0.0f); 
    glPushMatrix();

    Vector3 textPos = m_position + Vector3(-4.5f, 6.0f, 1.0f);
    glTranslatef(textPos.x, textPos.y, textPos.z);
    glScalef(0.02f, 0.02f, 0.02f);

    std::string text = "Welcome\n Car Showroom";
    for (char c : text) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); 
}