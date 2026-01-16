#include <windows.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Scene/IntroScene.h"
#include "Graphics/Renderer.h"

IntroScene* g_currentScene = nullptr;

//bool g_firstMouse = true;
//float g_lastX = 1024.0f / 2.0f;
//float g_lastY = 768.0f / 2.0f;

static void resize(int width, int height) {
    const float ar = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 2000.0);
    //gluPerspective(60.0, ar, 0.1, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}

static void display(void) {
    if (g_currentScene) {
        g_currentScene->render();
    }
    glutSwapBuffers();
}

static void idle(void) {
    if (g_currentScene) {
        g_currentScene->update(0.016f);
    }
    glutPostRedisplay();
}

static void key(unsigned char key, int x, int y) {
    if (g_currentScene) g_currentScene->handleKey(key);
    if (key == 27 || key == 'q') exit(0);
    glutPostRedisplay();
}

static void keyUp(unsigned char key, int x, int y) {
    if (g_currentScene) g_currentScene->handleKeyUp(key);
}

static void mouseMotion(int x, int y) {
    int centerX = 1024 / 2;
    int centerY = 768 / 2;

    float xoffset = x - centerX;
    float yoffset = centerY - y;

    if (g_currentScene)
        g_currentScene->handleMouse(xoffset, yoffset);

    glutWarpPointer(centerX, centerY);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Car Showroom - Free Camera");
    glutSetCursor(GLUT_CURSOR_NONE);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    g_currentScene = new IntroScene();
    glutMainLoop();
    delete g_currentScene;
    return EXIT_SUCCESS;
}