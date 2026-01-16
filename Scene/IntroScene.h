#pragma once
#include "Scene.h"
#include "../Graphics/Camera.h"
#include "../World/Environment.h"
#include "../World/Road.h"
#include "../World/Footpath.h"
#include "../World/Building.h"
#include "../World/Tree.h"
#include "../World/Skydome.h"
#include "../World/Arch.h" 
#include "../World/Bush.h"
#include "../World/Showroom.h"
#include <vector>
#include <map>


class IntroScene : public Scene {
public:
    IntroScene();
    void update(float deltaTime) override;
    void render() override;

    void handleKey(unsigned char key);
    void handleKeyUp(unsigned char key);
    void handleMouse(float xoffset, float yoffset);

private:
    void loadTextures();
    void setupWorld();
    void setupLighting();
    //void checkDoorProximity();

    Camera m_camera;
    Environment m_environment;
    Road m_road;
    Footpath m_footpath;
    std::vector<Building> m_buildings;
    std::vector<Tree> m_trees;
    std::vector<Bush> m_bushes; 
    Skydome* m_skydome;
    Arch* m_welcomeArch; 
    Showroom* m_showroom;
    std::map<unsigned char, bool> m_keys;
    bool m_isNight = false;

    unsigned int m_roadTex, m_grassTex, m_rockTex, m_footpathTex;
    unsigned int m_buildingTex1, m_buildingTex2, m_buildingTex3, m_buildingTex4;
    unsigned int m_treeBarkTex, m_treeLeavesTex;
    unsigned int m_archTextureID; 
    unsigned int m_starsTextureID;
    unsigned int m_bushTextureID;
};