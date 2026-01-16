#include <GL/freeglut.h>
#include "IntroScene.h"
#include "../Graphics/Renderer.h"
#include <cstdlib>
#include <iostream>

IntroScene::IntroScene() {
    loadTextures();
    setupWorld();
    m_skydome = new Skydome(m_starsTextureID);
    m_welcomeArch = new Arch(Vector3(0.0f, 0.0f, 40.0f), m_archTextureID);
    m_showroom = new Showroom(Vector3(0.0f, -1.5f, -180.0f));
}

void IntroScene::loadTextures() {
    Renderer::loadTexture("Assets/road.bmp", m_roadTex);
    Renderer::loadTexture("Assets/grass.bmp", m_grassTex);
    Renderer::loadTexture("Assets/rock.bmp", m_rockTex);
    Renderer::loadTexture("Assets/pavement1.jpg", m_footpathTex);
    Renderer::loadTexture("Assets/lastbuilding.bmp", m_buildingTex1);
    Renderer::loadTexture("Assets/thirdbuilding1.bmp", m_buildingTex2);
    Renderer::loadTexture("Assets/buildings3.jpg", m_buildingTex3);
    Renderer::loadTexture("Assets/buildings4.jpg", m_buildingTex4);
    Renderer::loadTexture("Assets/treeside.bmp", m_treeBarkTex);
    Renderer::loadTexture("Assets/grass.bmp", m_treeLeavesTex);

    Renderer::loadTexture("Assets/marble.jpg", m_bushTextureID);

    Renderer::loadTexture("Assets/marble.jpg", m_archTextureID);
    Renderer::loadTexture("Assets/stars.jpg", m_starsTextureID);
}

void IntroScene::setupWorld() {
    m_environment.setTextures(m_grassTex, m_rockTex);
    m_road.setTexture(m_roadTex);
    m_footpath.setTexture(m_footpathTex);
    srand(5);

    for (int i = 0; i < 2; ++i) {
        float z_pos = -i * 100.0f;
        unsigned int tex1 = m_buildingTex1, tex2 = m_buildingTex2, tex3 = m_buildingTex3, tex4 = m_buildingTex4;
        m_buildings.push_back(Building(Vector3(15, -4, z_pos), 10, (rand() % 13) + 7, 10, tex1));
        m_buildings.push_back(Building(Vector3(15, -4, z_pos + 25), 10, (rand() % 13) + 7, 10, tex2));
        m_buildings.push_back(Building(Vector3(15, -4, z_pos + 50), 10, (rand() % 13) + 7, 10, tex3));
        m_buildings.push_back(Building(Vector3(15, -4, z_pos + 75), 10, (rand() % 13) + 7, 10, tex4));
        m_buildings.push_back(Building(Vector3(-25, -4, z_pos), 10, (rand() % 13) + 7, 10, tex4));
        m_buildings.push_back(Building(Vector3(-25, -4, z_pos + 25), 10, (rand() % 13) + 7, 10, tex3));
        m_buildings.push_back(Building(Vector3(-25, -4, z_pos + 50), 10, (rand() % 13) + 7, 10, tex2));
        m_buildings.push_back(Building(Vector3(-25, -4, z_pos + 75), 10, (rand() % 13) + 7, 10, tex1));
        m_trees.push_back(Tree(Vector3(-8.5 - 3.7, -2.9, -10 * i * 10)));
        m_trees.push_back(Tree(Vector3(10 + 2.8, -2.9, -10 * i * 10)));
    }
    for (auto& tree : m_trees) {
        tree.setTextures(m_treeBarkTex, m_treeLeavesTex);
    }
}

void IntroScene::update(float deltaTime) {
    if (m_keys['w'] || m_keys['W']) m_camera.ProcessKeyboard(0, deltaTime);
    if (m_keys['s'] || m_keys['S']) m_camera.ProcessKeyboard(1, deltaTime);
    if (m_keys['a'] || m_keys['A']) m_camera.ProcessKeyboard(2, deltaTime);
    if (m_keys['d'] || m_keys['D']) m_camera.ProcessKeyboard(3, deltaTime);
    if (m_showroom) m_showroom->updateDoorState(m_camera.Position);
}

void IntroScene::render() {
    if (m_isNight) {
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    }
    else {
        glClearColor(0.2f, 0.593f, 0.85540f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    m_camera.applyViewMatrix();

    setupLighting();

    if (m_isNight) {
        m_skydome->draw(m_camera.Position);
    }

    m_environment.draw();
    m_road.draw();
    m_footpath.draw();
    //m_welcomeArch->draw();

    for (auto& building : m_buildings) building.draw();
    for (auto& tree : m_trees) tree.draw();
    for (auto& bush : m_bushes) bush.draw(); 
   if (m_showroom) m_showroom->draw();
}

void IntroScene::setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient_color[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    if (m_isNight) {
        ambient_color[0] = 0.05f; ambient_color[1] = 0.05f; ambient_color[2] = 0.1f;
    }
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);

    GLfloat light_pos[] = { 0.0f, 100.0f, 50.0f, 1.0f };
    GLfloat light_color[] = { 1.0f, 0.95f, 0.8f, 1.0f };
    if (m_isNight) {
        light_pos[1] = 50.0f;
        light_color[0] = 0.2f; light_color[1] = 0.2f; light_color[2] = 0.4f;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

    if (!m_isNight) {
        glEnable(GL_LIGHT1);
        GLfloat showroom_light_pos[] = { -10.0f, 10.0f, -180.0f, 1.0f };
        GLfloat showroom_light_color[] = { 0.8f, 0.8f, 1.0f, 1.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, showroom_light_pos);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, showroom_light_color);
        glLightfv(GL_LIGHT1, GL_SPECULAR, showroom_light_color);
    }
   /* if (m_showroom && m_showroom->areDoorsOpen()) {
        glEnable(GL_LIGHT1);
        GLfloat showroom_light_pos[] = { 0.0f, 5.0f, -180.0f, 1.0f };
        GLfloat showroom_light_color[] = { 0.9f, 0.9f, 1.0f, 1.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, showroom_light_pos);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, showroom_light_color);
        glLightfv(GL_LIGHT1, GL_SPECULAR, showroom_light_color);
    }
    else {
        glDisable(GL_LIGHT1);
    }*/

}

void IntroScene::handleKey(unsigned char key) {
    m_keys[key] = true;
    if (key == 'n' || key == 'N') {
        m_isNight = !m_isNight;
    }
}

void IntroScene::handleKeyUp(unsigned char key) {
    m_keys[key] = false;
}

void IntroScene::handleMouse(float xoffset, float yoffset) {
    m_camera.ProcessMouseMovement(xoffset, yoffset);
}

