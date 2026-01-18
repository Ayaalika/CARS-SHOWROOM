#include "Showroom.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "../Graphics/Renderer.h"
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#include "../tiny_obj_loader.h"

Showroom::Showroom(const Vector3& position) : m_position(position), doorPosition(0.0f), doorOpening(false), doorClosing(false),
m_scaleFactor(2.0f), archDisplayList(0), lamppostDisplayList(0) {
    lightPos[0] = 0.0f; lightPos[1] = 15.0f; lightPos[2] = 0.0f; lightPos[3] = 1.0f;
    interiorLightPos[0] = 0.0f; interiorLightPos[1] = 10.0f; interiorLightPos[2] = 0.0f; interiorLightPos[3] = 1.0f;
    spotLightPos[0] = 0.0f; spotLightPos[1] = 8.0f; spotLightPos[2] = 0.0f; spotLightPos[3] = 1.0f;
    spotLightDir[0] = 0.0f; spotLightDir[1] = -1.0f; spotLightDir[2] = 0.0f;
    Renderer::loadTexture("Assets/pavement.jpg", grassTexture);
    loadModels();

    m_buildingWidth = 10.0f;
    m_buildingLength = 12.0f;

    m_wallGap = 0.05f;
    m_corridorWidth = 1.7f; 
    float availableRoomWidth = m_buildingWidth - m_corridorWidth - (3 * m_wallGap);
    float availableRoomDepth = m_buildingLength - (3 * m_wallGap);
    m_roomWidth = availableRoomWidth / 2.0f;
    m_roomDepth = availableRoomDepth / 2.0f;
    

    roomCoords[0][0] = -m_buildingWidth / 2 + m_wallGap; 
    roomCoords[0][1] = -m_buildingLength / 2 + m_wallGap;

    roomCoords[1][0] = roomCoords[0][0] + m_roomWidth + m_wallGap + m_corridorWidth; 
    roomCoords[1][1] = -m_buildingLength / 2 + m_wallGap;

    roomCoords[2][0] = -m_buildingWidth / 2 + m_wallGap;
    roomCoords[2][1] = roomCoords[0][1] + m_roomDepth + m_wallGap;

    roomCoords[3][0] = roomCoords[2][0] + m_roomWidth + m_wallGap + m_corridorWidth; 
    roomCoords[3][1] = roomCoords[2][1];

    sportCarRoom = new SportCarRoom(Vector3(roomCoords[0][0], 0.0f, roomCoords[0][1]), m_roomWidth, m_roomDepth, 4.5f);
    familyCarRoom = new FamilyCarRoom(Vector3(roomCoords[1][0], 0.0f, roomCoords[1][1]), m_roomWidth, m_roomDepth, 4.5f);
    classicCarRoom = new ClassicCarRoom(Vector3(roomCoords[2][0], 0.0f, roomCoords[2][1]), m_roomWidth, m_roomDepth, 4.5f);
    suvCarRoom = new SUVCarRoom(Vector3(roomCoords[3][0], 0.0f, roomCoords[3][1]), m_roomWidth, m_roomDepth, 4.5f);

    rooms.push_back(sportCarRoom);
    rooms.push_back(familyCarRoom);
    rooms.push_back(classicCarRoom);
    rooms.push_back(suvCarRoom);

    loadRoomAssets();
}

Showroom::~Showroom() {
    delete sportCarRoom;
    delete familyCarRoom;
    delete classicCarRoom;
    delete suvCarRoom;
}

void Showroom::loadRoomAssets() {
    for (Room* room : rooms) {
        room->loadAssets();
    }
}

void Showroom::updateDoorState(const Vector3& cameraPosition) {
    float buildingLength = 12.0f;
    float doorWorldZ = m_position.z + (buildingLength * m_scaleFactor);

    float dx = cameraPosition.x - m_position.x;
    float dy = cameraPosition.y - 2.0f;
    float dz = cameraPosition.z - doorWorldZ; 
    float distanceToDoor = sqrt(dx * dx + dy * dy + dz * dz);

    if (distanceToDoor < DOOR_OPEN_DISTANCE && doorPosition < 1.0f) {
        doorOpening = true;
        doorClosing = false;
    }
    else if (distanceToDoor >= DOOR_OPEN_DISTANCE && doorPosition > 0.0f) {
        doorOpening = false;
        doorClosing = true;
    }

    if (doorOpening && doorPosition < 1.0f) {
        doorPosition += DOOR_SPEED;
        if (doorPosition > 1.0f) doorPosition = 1.0f;
    }
    else if (doorClosing && doorPosition > 0.0f) {
        doorPosition -= DOOR_SPEED;
        if (doorPosition < 0.0f) doorPosition = 0.0f;
    }
    float roomScaleFactor = m_scaleFactor * 1.95f;
    for (Room* room : rooms) {
        room->updateDoorState(cameraPosition, roomScaleFactor, m_position);
    }
}

void Showroom::draw() {
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    if (archDisplayList != 0) {
        GLfloat arch_ambient[] = { 0.2f, 0.25f, 0.35f, 1.0f };
        GLfloat arch_diffuse[] = { 0.4f, 0.5f, 0.7f, 1.0f };
        GLfloat arch_specular[] = { 0.5f, 0.5f, 0.6f, 1.0f };
        GLfloat arch_shininess = 60.0f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, arch_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, arch_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, arch_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, arch_shininess);
        drawModel(archDisplayList, -9.5f, 0.0f, -65.0f, 1.0f);

        GLfloat neutral_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat neutral_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, neutral_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, neutral_diffuse);
    }
    else {
        drawArch(0.0f, 0.0f, -150.0f, 1.0f);
    }
    if (lamppostDisplayList != 0) {
        drawModel(lamppostDisplayList, 14.0f, 0.0f, -65.0f, 0.03f);
        drawModel(lamppostDisplayList, -14.0f, 0.0f, -65.0f, 0.03f);
    }
    else {
        drawLamppost(15.0f, 0.0f, -35.0f, 0.100f);
        drawLamppost(-15.0f, 0.0f, -35.0f, 0.100f);
    }
    drawRoad();
    drawSidewalk();
    drawGrass();

    for (int i = 0; i < 6; i++) {
        float zPos = -55.0f + i * 5.0f;
        drawRealisticTree(7.0f, 0.0f, zPos, 1.0f);
        drawRealisticTree(-7.0f, 0.0f, zPos, 1.0f);
    }

    drawShowroomFloor();

    glPushMatrix();
    glScalef(m_scaleFactor, 1.0f, m_scaleFactor);

    float buildingWidth = 10.0f;
    float buildingLength = 12.0f;
    float wallHeight = 7.5f;
    float roofOverhang = 3.0f;
    float frameThick = 0.2f;

    drawReflectiveWall(-buildingWidth, 0.0f, -buildingLength, (buildingWidth - 1.5f), wallHeight, frameThick, false);
    drawReflectiveWall(1.5f, 0.0f, -buildingLength, (buildingWidth - 1.5f), wallHeight, frameThick, false);
    drawReflectiveWall(-buildingWidth, 0.0f, buildingLength, buildingWidth * 2.0f, wallHeight, frameThick, false);
    drawReflectiveWall(-buildingWidth, 0.0f, -buildingLength, buildingLength * 2.0f, wallHeight, frameThick, true);
    drawReflectiveWall(buildingWidth, 0.0f, -buildingLength, buildingLength * 2.0f, wallHeight, frameThick, true);

    float roofThickness = 0.5f;
    float lightLayerHeight = wallHeight - 0.05f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 1.0f, 0.9f, 0.4f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-buildingWidth - roofOverhang, lightLayerHeight, -buildingLength - roofOverhang);
    glVertex3f(buildingWidth + roofOverhang, lightLayerHeight, -buildingLength - roofOverhang);
    glVertex3f(buildingWidth + roofOverhang, lightLayerHeight, buildingLength + roofOverhang);
    glVertex3f(-buildingWidth - roofOverhang, lightLayerHeight, buildingLength + roofOverhang);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-buildingWidth - roofOverhang, wallHeight, -buildingLength - roofOverhang);
    glVertex3f(buildingWidth + roofOverhang, wallHeight, -buildingLength - roofOverhang);
    glVertex3f(buildingWidth + roofOverhang, wallHeight, buildingLength + roofOverhang);
    glVertex3f(-buildingWidth - roofOverhang, wallHeight, buildingLength + roofOverhang);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-buildingWidth - roofOverhang, wallHeight + 0.5f - roofThickness, -buildingLength - roofOverhang);
    glVertex3f(buildingWidth + roofOverhang, wallHeight + 0.5f - roofThickness, -buildingLength - roofOverhang);
    glVertex3f(buildingWidth + roofOverhang, wallHeight + 0.5f, -buildingLength - roofOverhang);
    glVertex3f(-buildingWidth - roofOverhang, wallHeight + 0.5f, -buildingLength - roofOverhang);
    glEnd();

    float doorHeight = wallHeight * 0.85f;
    drawSlidingDoor(doorHeight);

    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, wallHeight + 0.5f, -buildingLength - roofOverhang - 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-0.8f, 0.0f, -0.5f);
    glVertex3f(0.8f, 0.0f, -0.5f);
    glVertex3f(0.8f, 0.0f, 0.5f);
    glVertex3f(-0.8f, 0.0f, 0.5f);
    glEnd();

    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.4f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex3f(-0.15f, 0.0f, -0.15f);
        glVertex3f(0.15f, 0.0f, -0.15f);
        glVertex3f(0.15f, 0.0f, -0.15f);
        glVertex3f(-0.15f, 0.0f, -0.15f);

        glVertex3f(-0.15f, 0.0f, 0.15f);
        glVertex3f(0.15f, 0.0f, 0.15f);
        glVertex3f(0.15f, 0.0f, 0.15f);
        glVertex3f(-0.15f, 0.0f, 0.15f);

        glVertex3f(-0.15f, 0.0f, -0.15f);
        glVertex3f(-0.15f, 0.0f, 0.15f);
        glVertex3f(-0.15f, 0.0f, 0.15f);
        glVertex3f(-0.15f, 0.0f, -0.15f);

        glVertex3f(0.15f, 0.0f, -0.15f);
        glVertex3f(0.15f, 0.0f, 0.15f);
        glVertex3f(0.15f, 0.0f, 0.15f);
        glVertex3f(0.15f, 0.0f, -0.15f);
        glEnd();
        glPopMatrix();
    }

    glBegin(GL_QUADS);
    glVertex3f(-0.5f, 3.0f, -0.2f);
    glVertex3f(0.5f, 3.0f, -0.2f);
    glVertex3f(0.5f, 3.2f, -0.2f);
    glVertex3f(-0.5f, 3.2f, -0.2f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-4.0f, 2.9f, -0.25f);
    glVertex3f(4.0f, 2.9f, -0.25f);
    glVertex3f(4.0f, 3.1f, -0.25f);
    glVertex3f(-4.0f, 3.1f, -0.25f);
    glEnd();

    drawNeonSign();
    glPopMatrix();
    glPushMatrix();
   glMatrixMode(GL_MODELVIEW);
    for (Room* room : rooms) {
        glPushMatrix();
        glScalef(m_scaleFactor, 1.0f, m_scaleFactor);
        room->draw();
        glPopMatrix();
    }
    glPopMatrix();
    glColor3f(0.15f, 0.15f, 0.15f);

    float pillarOffset = 10.5f;
    drawPillar(-buildingWidth - roofOverhang - pillarOffset, -buildingLength - roofOverhang - pillarOffset, wallHeight, 0.5f);
    drawPillar(buildingWidth + roofOverhang + pillarOffset, -buildingLength - roofOverhang - pillarOffset, wallHeight, 0.5f);
    drawPillar(-buildingWidth - roofOverhang - pillarOffset, buildingLength + roofOverhang + pillarOffset, wallHeight, 0.5f);
    drawPillar(buildingWidth + roofOverhang + pillarOffset, buildingLength + roofOverhang + pillarOffset, wallHeight, 0.5f);

    drawSpotlightEffect(-buildingWidth / 2, wallHeight - 0.3f, -buildingLength / 2);
    drawSpotlightEffect(buildingWidth / 2, wallHeight - 0.3f, -buildingLength / 2);
    drawSpotlightEffect(0.0f, wallHeight - 0.3f, buildingLength / 2);
    drawSpotlightEffect(-buildingWidth / 2, wallHeight - 0.3f, buildingLength / 2);
    drawSpotlightEffect(buildingWidth / 2, wallHeight - 0.3f, buildingLength / 2);
    drawSpotlightEffect(0.0f, wallHeight - 0.3f, 0.0f);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -180.0f);
    drawShadow();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -180.0f);
    drawReflection();
    glPopMatrix();
}

void Showroom::loadModels() {
    if (loadModel("Assets/Models/arch.obj", archAttrib, archShapes, archMaterials)) {
        archDisplayList = createDisplayList(archAttrib, archShapes, archMaterials, 1.0f);
    }
    else {
       //std::cerr << "Failed to load arch model. Using procedural drawing." << std::endl;
        archDisplayList = 0;
    }

    if (loadModel("Assets/Models/Obj/lamppost.obj", lamppostAttrib, lamppostShapes, lamppostMaterials, "lamppost.mtl")) {
        lamppostDisplayList = createDisplayList(lamppostAttrib, lamppostShapes, lamppostMaterials, 1.0f);
    }
    else {
        std::cerr << "Failed to load lamppost model. Using procedural drawing." << std::endl;
        lamppostDisplayList = 0;
    }
}

bool Showroom::loadModel(const std::string& filename, tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials, const std::string& mtl_filename_override) {
    std::string warn;
    std::string err;

    std::string base_dir = filename.substr(0, filename.find_last_of("/\\") + 1);

    bool ret;
    if (mtl_filename_override.empty()) {
        ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), base_dir.c_str());
    }
    else {
        ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), base_dir.c_str(), true, mtl_filename_override.c_str());
    }

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "ERR: " << err << std::endl;
    }

    if (!ret && shapes.empty()) {
        std::cerr << "Failed to load " << filename << " (no geometry found)." << std::endl;
        return false;
    }
    else if (!ret) {
        std::cerr << "Failed to load materials for " << filename << ", but geometry loaded. Using default materials." << std::endl;
        return true;
    }

    return ret;
}

GLuint Showroom::createDisplayList(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials, float scale) {
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    glEnable(GL_LIGHTING);

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        int current_material_id = -1;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            int material_id = shapes[s].mesh.material_ids[f];

            if (material_id != current_material_id && material_id >= 0 && material_id < materials.size()) {
                current_material_id = material_id;
                const tinyobj::material_t& mat = materials[material_id];

                if (!mat.diffuse_texname.empty()) {
                    if (m_textureMap.find(mat.diffuse_texname) == m_textureMap.end()) {
                        std::string texturePath = "Assets/Models/Obj/" + mat.diffuse_texname;
                        GLuint texID;
                        Renderer::loadTexture(texturePath.c_str(), texID);
                        if (texID != 0) {
                            m_textureMap[mat.diffuse_texname] = texID;
                        }
                        else {
                            std::cerr << "Failed to load texture: " << texturePath << std::endl;
                        }
                    }
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, m_textureMap[mat.diffuse_texname]);
                }
                else {
                    glDisable(GL_TEXTURE_2D);
                }
                GLfloat ambient[4] = { mat.ambient[0], mat.ambient[1], mat.ambient[2], 1.0f };
                GLfloat diffuse[4] = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1.0f };
                GLfloat specular[4] = { mat.specular[0], mat.specular[1], mat.specular[2], 1.0f };
                GLfloat shininess = mat.shininess;

                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
            }

            glBegin(GL_TRIANGLES);
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0] * scale;
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1] * scale;
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2] * scale;

                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                    glTexCoord2f(tx, ty);
                }
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                    tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                    tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
                    glNormal3f(nx, ny, nz);
                }

                glVertex3f(vx, vy, vz);
            }
            glEnd();

            index_offset += fv;
        }
    }
    glDisable(GL_TEXTURE_2D);
    glEndList();
    return list;
}

void Showroom::drawModel(GLuint displayList, float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);
    glCallList(displayList);
    glPopMatrix();
}

void Showroom::drawNeonSign() {
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, -0.25f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    float glowIntensity = 0.9f + 0.1f * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
    glColor3f(0.8f, 0.8f, 1.0f);
    glLineWidth(3.0f);
    glPushMatrix();
    glScalef(0.01f, 0.01f, 0.01f);
    std::string text = "CARS SHOWROOM";
    float textWidth = 0;
    for (char c : text) {
        textWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, c);
    }
    glTranslatef(-textWidth / 2, 0.0f, 0.0f);
    for (char c : text) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.8f, 0.8f, 1.0f, 0.3f * glowIntensity);
    glLineWidth(8.0f);
    //glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glScalef(0.01f, 0.01f, 0.01f);
    glTranslatef(-textWidth / 2, 0.0f, 0.0f);
    for (char c : text) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();
    glDisable(GL_BLEND);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void Showroom::drawNeonLetter(char letter, float x, float y, float z, float size, float glow) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(size, size, size);
    glColor3f(1.0f * glow, 1.0f * glow, 1.0f * glow);
    glLineWidth(8.0f);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, letter);
    glPopMatrix();
}

void Showroom::drawRoad() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-3.0f, 0.01f, -65.0f);
    glVertex3f(3.0f, 0.01f, -65.0f);
    glVertex3f(3.0f, 0.01f, -25.0f);
    glVertex3f(-3.0f, 0.01f, -25.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (float z = -63.0f; z < -25.0f; z += 4.0f) {
        glVertex3f(0.0f, 0.02f, z);
        glVertex3f(0.0f, 0.02f, z + 2.0f);
    }
    glEnd();
}

void Showroom::drawGrass() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 0.08f, -65.0f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(15.0f, 0.08f, -65.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(15.0f, 0.08f, -25.0f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(5.0f, 0.08f, -25.0f);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f, 0.08f, -65.0f);
    glTexCoord2f(10.0f, 0.0f); glVertex3f(-5.0f, 0.08f, -65.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f(-5.0f, 0.08f, -25.0f);
    glTexCoord2f(0.0f, 10.0f); glVertex3f(-15.0f, 0.08f, -25.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Showroom::drawSidewalk() {
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(3.0f, 0.08f, -65.0f);
    glVertex3f(5.0f, 0.08f, -65.0f);
    glVertex3f(5.0f, 0.08f, -25.0f);
    glVertex3f(3.0f, 0.08f, -25.0f);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-5.0f, 0.08f, -65.0f);
    glVertex3f(-3.0f, 0.08f, -65.0f);
    glVertex3f(-3.0f, 0.08f, -25.0f);
    glVertex3f(-5.0f, 0.08f, -25.0f);
    glEnd();
}

void Showroom::drawShowroomFloor() {
    GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat mat_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat mat_shininess[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 0.0f, -50.0f);
    glTexCoord2f(20.0f, 0.0f); glVertex3f(50.0f, 0.0f, -50.0f);
    glTexCoord2f(20.0f, 20.0f); glVertex3f(50.0f, 0.0f, 50.0f);
    glTexCoord2f(0.0f, 20.0f); glVertex3f(-50.0f, 0.0f, 50.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.35f, 0.35f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = -40; i <= 40; i += 5) {
        glVertex3f(static_cast<GLfloat>(i), 0.001f, -40.0f);
        glVertex3f(static_cast<GLfloat>(i), 0.001f, 40.0f);

        glVertex3f(-40.0f, 0.001f, static_cast<GLfloat>(i));
        glVertex3f(40.0f, 0.001f, static_cast<GLfloat>(i));
    }
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-3.0f, 0.01f, -2.0f);
    glVertex3f(3.0f, 0.01f, -2.0f);
    glVertex3f(3.0f, 0.01f, 2.0f);
    glVertex3f(-3.0f, 0.01f, 2.0f);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-3.0f, 0.01f, 2.0f);
    glVertex3f(3.0f, 0.01f, 2.0f);
    glVertex3f(3.0f, 0.1f, 2.0f);
    glVertex3f(-3.0f, 0.1f, 2.0f);

    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-3.0f, 0.01f, -2.0f);
    glVertex3f(3.0f, 0.01f, -2.0f);
    glVertex3f(3.0f, 0.1f, -2.0f);
    glVertex3f(-3.0f, 0.1f, -2.0f);

    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(3.0f, 0.01f, -2.0f);
    glVertex3f(3.0f, 0.01f, 2.0f);
    glVertex3f(3.0f, 0.1f, 2.0f);
    glVertex3f(3.0f, 0.1f, -2.0f);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-3.0f, 0.01f, -2.0f);
    glVertex3f(-3.0f, 0.01f, 2.0f);
    glVertex3f(-3.0f, 0.1f, 2.0f);
    glVertex3f(-3.0f, 0.1f, -2.0f);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 1.0f, 0.9f, 0.3f);
    for (int i = -2; i <= 2; i++) {
        for (int j = -1; j <= 1; j++) {
            glPushMatrix();
            glTranslatef(i * 1.5f, 0.02f, j * 1.5f);
            glutSolidSphere(0.1f, 10, 10);
            glPopMatrix();
        }
    }
    glDisable(GL_BLEND);
}

void Showroom::drawReflection() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.3f, 0.3f, 0.3f, 0.3f);

    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(0.0f, -1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    //glRotatef(myCar.rotation, 0, 1, 0);

    glBegin(GL_QUADS);
    glVertex3f(-1.8f, 0.45f, -0.75f);
    glVertex3f(1.8f, 0.45f, -0.75f);
    glVertex3f(1.8f, 0.45f, 0.75f);
    glVertex3f(-1.8f, 0.45f, 0.75f);
    glEnd();

    glPopMatrix();
    glPopMatrix();
    glDisable(GL_BLEND);
}

void Showroom::drawShadow() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 0.01f, 0.0f);
    glScalef(1.0f, 0.01f, 1.0f);

    glBegin(GL_QUADS);
    glVertex3f(-2.0f, 0.0f, -1.0f);
    glVertex3f(2.0f, 0.0f, -1.0f);
    glVertex3f(2.0f, 0.0f, 1.0f);
    glVertex3f(-2.0f, 0.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void Showroom::drawTree(float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);

    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.2f, 1.0f, 10, 10);
    glPopMatrix();

    glColor3f(0.1f, 0.6f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(1.0f, 1.5f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.8f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.8f, 1.2f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 2.2f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.6f, 1.0f, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

void Showroom::drawRealisticTree(float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);

    GLfloat trunk_ambient[] = { 0.15f, 0.08f, 0.03f, 1.0f };
    GLfloat trunk_diffuse[] = { 0.3f, 0.15f, 0.05f, 1.0f };
    GLfloat trunk_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat trunk_shininess[] = { 10.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, trunk_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, trunk_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, trunk_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, trunk_shininess);

    glColor3f(0.3f, 0.15f, 0.05f);
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    for (int i = 0; i < 5; i++) {
        float radius = 0.25f - i * 0.03f;
        float height = 0.2f;
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, i * height);
        glutSolidCylinder(radius, height, 15, 5);
        glPopMatrix();
    }
    glPopMatrix();

    GLfloat leaf_ambient[] = { 0.05f, 0.2f, 0.05f, 1.0f };
    GLfloat leaf_diffuse[] = { 0.1f, 0.5f, 0.1f, 1.0f };
    GLfloat leaf_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat leaf_shininess[] = { 5.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, leaf_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, leaf_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, leaf_shininess);

    glColor3f(0.1f, 0.5f, 0.1f);
    srand(123);

    for (int i = 0; i < 15; i++) {
        float x_offset = (rand() % 200 - 100) / 100.0f;
        float y_offset = (rand() % 100) / 100.0f;
        float z_offset = (rand() % 200 - 100) / 100.0f;
        float size = 0.3f + (rand() % 100) / 200.0f;

        glPushMatrix();
        glTranslatef(x_offset, 1.2f + y_offset, z_offset);
        glutSolidSphere(size, 10, 10);
        glPopMatrix();
    }

    glPopMatrix();
}

void Showroom::drawLamppost(float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);

    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.1f, 5.0f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 4.5f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.05f, 1.0f, 10, 10);
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 1.0f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(1.0f, 4.5f, 0.0f);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();

    glColor4f(1.0f, 1.0f, 0.8f, 0.3f);
    glPushMatrix();
    glTranslatef(1.0f, 4.5f, 0.0f);
    glutSolidSphere(0.5f, 10, 10);
    glPopMatrix();

    glColor4f(1.0f, 1.0f, 0.8f, 0.2f);
    glPushMatrix();
    glTranslatef(1.0f, 4.5f, 0.0f);
    glRotatef(90, 0, 1, 0);
    glutSolidCone(1.0f, 3.0f, 10, 10);
    glPopMatrix();

    glDisable(GL_BLEND);
    glPopMatrix();
}

void Showroom::drawArch(float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);

    glColor3f(0.6f, 0.4f, 0.2f);
    glPushMatrix();
    glTranslatef(-2.0f, 2.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.3f, 5.0f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0f, 2.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.3f, 5.0f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 5.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidTorus(0.3f, 2.0f, 10, 20);
    glPopMatrix();

    glColor3f(0.8f, 0.6f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 5.5f, 0.0f);
    glutSolidSphere(0.2f, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

void Showroom::drawPillar(float x, float z, float h, float w) {
    GLfloat pillar_ambient[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    GLfloat pillar_diffuse[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    GLfloat pillar_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, pillar_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pillar_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pillar_specular);
    
    float hw = w / 2.0f;
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x - hw, 0.0f, z - hw);
    glVertex3f(x + hw, 0.0f, z - hw);
    glVertex3f(x + hw, h, z - hw);
    glVertex3f(x - hw, h, z - hw);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x - hw, 0.0f, z + hw);
    glVertex3f(x + hw, 0.0f, z + hw);
    glVertex3f(x + hw, h, z + hw);
    glVertex3f(x - hw, h, z + hw);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x - hw, 0.0f, z - hw);
    glVertex3f(x - hw, 0.0f, z + hw);
    glVertex3f(x - hw, h, z + hw);
    glVertex3f(x - hw, h, z - hw);

    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x + hw, 0.0f, z - hw);
    glVertex3f(x + hw, 0.0f, z + hw);
    glVertex3f(x + hw, h, z + hw);
    glVertex3f(x + hw, h, z - hw);
    glEnd();
}

void Showroom::drawReflectiveWall(float x, float y, float z, float width, float height, float thickness, bool isSideWall) {
    GLfloat wall_ambient[] = { 0.2f, 0.2f, 0.25f, 1.0f };
    GLfloat wall_diffuse[] = { 0.3f, 0.3f, 0.35f, 1.0f };
    GLfloat wall_specular[] = { 0.9f, 0.9f, 0.95f, 1.0f };
    GLfloat wall_shininess[] = { 120.0f };
    GLfloat wall_emission[] = { 0.1f, 0.1f, 0.15f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, wall_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wall_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wall_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wall_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, wall_emission);

    glColor3f(0.6f, 0.6f, 0.7f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (isSideWall) {
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + width);
        glVertex3f(x, y + height, z + width);
        glVertex3f(x, y + height, z);
        glEnd();
    }
    else {
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, y, z);
        glVertex3f(x + width, y, z);
        glVertex3f(x + width, y + height, z);
        glVertex3f(x, y + height, z);
        glEnd();
    }

    glDisable(GL_BLEND);

    glColor3f(0.05f, 0.05f, 0.05f);
    glLineWidth(thickness * 12.0f);
    glBegin(GL_LINE_LOOP);
    if (isSideWall) {
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + width);
        glVertex3f(x, y + height, z + width);
        glVertex3f(x, y + height, z);
    }
    else {
        glVertex3f(x, y, z);
        glVertex3f(x + width, y, z);
        glVertex3f(x + width, y + height, z);
        glVertex3f(x, y + height, z);
    }
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.8f, 0.8f, 1.0f, 0.2f);

    if (isSideWall) {
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(x + 0.01f, y + height * 0.7f, z + width * 0.3f);
        glVertex3f(x + 0.01f, y + height * 0.3f, z + width * 0.3f);
        glVertex3f(x + 0.01f, y + height * 0.3f, z + width * 0.7f);
        glVertex3f(x + 0.01f, y + height * 0.7f, z + width * 0.7f);
        glEnd();
    }
    else {
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x + width * 0.3f, y + height * 0.7f, z + 0.01f);
        glVertex3f(x + width * 0.7f, y + height * 0.7f, z + 0.01f);
        glVertex3f(x + width * 0.7f, y + height * 0.3f, z + 0.01f);
        glVertex3f(x + width * 0.3f, y + height * 0.3f, z + 0.01f);
        glEnd();
    }

    glDisable(GL_BLEND);
}

void Showroom::drawSlidingDoor(float doorHeight) {
    float doorWidth = 3.0f;
    float doorThickness = 0.25f;
    float frameThick = 0.3f;
    float buildingLength = 12.0f;
    float z = -buildingLength - doorThickness / 2; 
    float frameLeft = -doorWidth / 2;
    float frameRight = doorWidth / 2;

    glColor3f(0.18f, 0.18f, 0.18f);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex3f(frameLeft, 0.0f, z);
    glVertex3f(frameLeft, doorHeight, z);
    glVertex3f(frameRight, 0.0f, z);
    glVertex3f(frameRight, doorHeight, z);
    glVertex3f(frameLeft, doorHeight, z);
    glVertex3f(frameRight, doorHeight, z);
    glEnd();

    float halfWidth = doorWidth / 2.0f;
    float leftDoorStart = -halfWidth - doorPosition * halfWidth;
    float leftDoorEnd = 0.0f - doorPosition * halfWidth;
    float rightDoorStart = 0.0f + doorPosition * halfWidth;
    float rightDoorEnd = halfWidth + doorPosition * halfWidth;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.6f, 0.6f, 0.6f, 0.2f);

    glBegin(GL_QUADS);
    glVertex3f(leftDoorStart, 0.0f, z);
    glVertex3f(leftDoorEnd, 0.0f, z);
    glVertex3f(leftDoorEnd, doorHeight, z);
    glVertex3f(leftDoorStart, doorHeight, z);

    glVertex3f(rightDoorStart, 0.0f, z);
    glVertex3f(rightDoorEnd, 0.0f, z);
    glVertex3f(rightDoorEnd, doorHeight, z);
    glVertex3f(rightDoorStart, doorHeight, z);
    glEnd();

    glDisable(GL_BLEND);

    if (doorPosition > 0.01f) {
        glLineWidth(4.0f);
        glColor3f(0.18f, 0.18f, 0.18f);
        glBegin(GL_LINES);
        glVertex3f(leftDoorEnd, 0.0f, z);
        glVertex3f(leftDoorEnd, doorHeight, z);
        glEnd();
    }
}

void Showroom::drawModernCurvedFence(float radius, float height, int segments) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.6f, 0.8f, 1.0f, 0.2f);

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = -0.4f * 3.14159f + (i * (1.8f * 3.14159f / segments));
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;

        glVertex3f(x, 0.0f, z);
        glVertex3f(x, height, z);
    }
    glEnd();

    glDisable(GL_BLEND);
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(4.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = -0.4f * 3.14159f + (i * (1.8f * 3.14159f / segments));
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        glVertex3f(x, height, z);
    }
    glEnd();

    glBegin(GL_LINES);
    float a1 = -0.4f * 3.14159f;
    glVertex3f(cos(a1) * radius, 0, sin(a1) * radius);
    glVertex3f(cos(a1) * radius, height, sin(a1) * radius);

    float a2 = -0.4f * 3.14159f + (1.8f * 3.14159f);
    glVertex3f(cos(a2) * radius, 0, sin(a2) * radius);
    glVertex3f(cos(a2) * radius, height, sin(a2) * radius);
    glEnd();
}

void Showroom::drawSpotlightEffect(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.95f, 0.95f, 1.0f, 0.7f);
    glutSolidCone(0.4f, 0.7f, 10, 10);
    glDisable(GL_BLEND);

    glPopMatrix();
}

void Showroom::update(float deltaTime) {
    for (Room* room : rooms) {
        room->update(deltaTime);
    }
}