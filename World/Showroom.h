#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <GL/freeglut.h> 
#include "../tiny_obj_loader.h"
#include "../Math/Vector3.h" 
#include "../Scene/Room.h"
#include "../Scene/Rooms/SportCarRoom.h"
#include "../Scene/Rooms/FamilyCarRoom.h"
#include "../Scene/Rooms/ClassicCarRoom.h"
#include "../Scene/Rooms/SUVCarRoom.h"

#define DOOR_OPEN_DISTANCE 5.0f 
#define DOOR_SPEED 0.05f    

class Showroom {
public:
    Showroom(const Vector3& position);
    ~Showroom();
    void updateDoorState(const Vector3& cameraPosition);
    void draw();

    void update(float deltaTime);
private:
    Vector3 m_position;

    float doorPosition;
    bool doorOpening;
    bool doorClosing;

    float m_scaleFactor;

    float m_corridorWidth;
    float m_roomWidth;
    float m_roomDepth;
    float m_wallGap;

    float m_buildingWidth;
    float m_buildingLength;

    GLfloat lightPos[4];
    GLfloat interiorLightPos[4];
    GLfloat spotLightPos[4];
    GLfloat spotLightDir[3];

    GLuint grassTexture;
    GLuint floorTexture;

    GLuint archDisplayList;
    GLuint lamppostDisplayList;

    tinyobj::attrib_t archAttrib;
    std::vector<tinyobj::shape_t> archShapes;
    std::vector<tinyobj::material_t> archMaterials;

    tinyobj::attrib_t lamppostAttrib;
    std::vector<tinyobj::shape_t> lamppostShapes;
    std::vector<tinyobj::material_t> lamppostMaterials;
    std::map<std::string, GLuint> m_textureMap;
 
    SportCarRoom* sportCarRoom;
    FamilyCarRoom* familyCarRoom;
    ClassicCarRoom* classicCarRoom;
    SUVCarRoom* suvCarRoom;
    std::vector<Room*> rooms;

    void loadRoomAssets();

    void loadModels();
    bool loadModel(const std::string& filename, tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials, const std::string& mtl_filename_override = "");
    GLuint createDisplayList(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials, float scale);
    void drawModel(GLuint displayList, float x, float y, float z, float scale);

    void drawShowroomFloor();
    void drawSlidingDoor(float doorHeight);

    void drawReflectiveWall(float x, float y, float z, float width, float height, float thickness, bool isSideWall);
    void drawNeonSign();
    void drawNeonLetter(char letter, float x, float y, float z, float size, float glow);
    void drawRoad();
    void drawGrass();
    void drawSidewalk();
    void drawPillar(float x, float z, float h, float w);
    void drawSpotlightEffect(float x, float y, float z);
    void drawModernCurvedFence(float radius, float height, int segments);

    void drawTree(float x, float y, float z, float scale);
    void drawRealisticTree(float x, float y, float z, float scale);
    void drawLamppost(float x, float y, float z, float scale);
    void drawArch(float x, float y, float z, float scale);

    void drawShadow();
    void drawReflection();
    float roomCoords[4][2];
};