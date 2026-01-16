#include <GL/freeglut.h>
#include "Renderer.h"
#include "Texture.h" 
#include <iostream>


static GLfloat v_cube[8][3] =
{
    {0,0,0}, {0,0,1}, {0,1,0}, {0,1,1},
    {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {3,1,5,7}, 
    {2,0,1,3}, 
    {7,5,4,6}, 
    {2,3,7,6}, 
    {1,0,4,5}, 
    {6,4,0,2}
};

void Renderer::loadTexture(const char* filename, unsigned int& textureID) {
    Texture texture;

    if (texture.Load(filename)) {
        textureID = texture.GetID();
    }
    else {
        std::cerr << "ERROR: Renderer failed to load texture: " << filename << std::endl;
        textureID = 0;
    }
}

void Renderer::drawCube(float cr, float cg, float cb, int n, bool e) {
    GLfloat m_no[] = { 0, 0, 0, 1.0 };
    GLfloat m_amb[] = { cr, cg, cb, 1.0 };
    GLfloat m_diff[] = { cr, cg, cb, 1.0 };
    GLfloat m_spec[] = { 1, 1, 1, 1.0 };
    GLfloat m_sh[] = { 90 };
    GLfloat m_em[] = { 1, 1, 1, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, m_sh);

    if (e) {
        glMaterialfv(GL_FRONT, GL_EMISSION, m_diff);
    }
    else {
        glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }

    glBegin(GL_QUADS);
    for (GLint i = 0; i < 6; i++) {
        Renderer::setNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
            v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
            v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glTexCoord2f(0, 0); glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(n, 0); glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(n, n); glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(0, n); glVertex3fv(&v_cube[c_ind[i][3]][0]);
    }
    glEnd();
}

void Renderer::setNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3) {
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2 - x1;
    Uy = y2 - y1;
    Uz = z2 - z1;

    Vx = x3 - x1;
    Vy = y3 - y1;
    Vz = z3 - z1;

    Nx = Uy * Vz - Uz * Vy;
    Ny = Uz * Vx - Ux * Vz;
    Nz = Ux * Vy - Uy * Vx;

    glNormal3f(Nx, Ny, Nz);
}

void Renderer::drawCylinder(float base, float top, float height) {
    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
    GLfloat mat_specular[] = { 1, 1, 1, 1.0 };
    GLfloat mat_shininess[] = { 90 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLUquadricObj* quadratic = gluNewQuadric();
    gluQuadricTexture(quadratic, GL_TRUE);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic, base, top, height, 32, 32);
    gluDeleteQuadric(quadratic);
}

void Renderer::drawSphere(float radius) {
    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
    GLfloat mat_specular[] = { 1, 1, 1, 1.0 };
    GLfloat mat_shininess[] = { 90 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLUquadric* qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, radius, 20, 20);
    gluDeleteQuadric(qobj);
}