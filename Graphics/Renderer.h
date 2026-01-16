#pragma once
#include <GL/gl.h>
#include <string>

class Renderer {
public:
    static void loadTexture(const char* filename, unsigned int& textureID);
    static void drawCube(float cr, float cg, float cb, int n = 1, bool e = false);
    static void setNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
    static void drawCylinder(float base, float top, float height);
    static void drawSphere(float radius);
};