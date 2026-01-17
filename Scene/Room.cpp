#include "Room.h"
#include <iostream>
#include <cmath>

const float Room::DOOR_OPEN_DISTANCE = 5.0f;
const float Room::DOOR_SPEED = 0.05f;

Room::Room(const Vector3& position, float width, float depth, float height)
    : m_position(position), m_width(width), m_depth(depth), m_height(height),
    doorPosition(0.0f), doorOpening(false), doorClosing(false) {
    
    carRotation = 0.0f;     
    rotationSpeed = 20.0f;
}

Room::~Room() {
}

void Room::updateDoorState(const Vector3& cameraPosition, float scaleFactor, const Vector3& showroomOrigin) {
    float doorLocalX, doorLocalZ;
    if (m_position.x < 0) { 
        doorLocalX = m_width; 
    }
    else {
        doorLocalX = 0.0f; 
    }
    doorLocalZ = m_depth / 2.0f; 

    float doorWorldX = showroomOrigin.x + (m_position.x + doorLocalX) * scaleFactor;
    float doorWorldZ = showroomOrigin.z + (m_position.z + doorLocalZ) * scaleFactor;

    float dx = cameraPosition.x - doorWorldX;
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
}

void Room::drawRoomBase() {
    const float DOOR_W = 1.8f, DOOR_H = 2.5f;

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);

    glEnable(GL_LIGHT2);
    GLfloat lightPos[] = { m_width / 2.0f, m_height - 0.5f, m_depth / 2.0f, 1.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambientColor);

    GLfloat whiteMat[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, whiteMat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteMat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteMat);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.01f, 0.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(m_width, 0.01f, 0.0f);
    glTexCoord2f(3.0f, 3.0f); glVertex3f(m_width, 0.01f, m_depth);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(0.0f, 0.01f, m_depth);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, m_depth);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(m_width, 0.0f, m_depth);
    glTexCoord2f(2.0f, 1.0f); glVertex3f(m_width, m_height, m_depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, m_height, m_depth);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(m_width, 0.0f, 0.0f);
    glTexCoord2f(2.0f, 1.0f); glVertex3f(m_width, m_height, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, m_height, 0.0f);
    glEnd();

    bool isLeftRoom = (m_position.x < 0);
    if (isLeftRoom) {
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, m_depth);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
        glTexCoord2f(2.0f, 1.0f); glVertex3f(0.0f, m_height, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, m_height, m_depth);
        glEnd();
    }
    else {
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(m_width, 0.0f, 0.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(m_width, 0.0f, m_depth);
        glTexCoord2f(2.0f, 1.0f); glVertex3f(m_width, m_height, m_depth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(m_width, m_height, 0.0f);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambientColor);
    glBindTexture(GL_TEXTURE_2D, ceilingTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, m_height, 0.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(m_width, m_height, 0.0f);
    glTexCoord2f(3.0f, 3.0f); glVertex3f(m_width, m_height, m_depth);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(0.0f, m_height, m_depth);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    drawNeonCeiling();
    drawPlatform();
    drawGlassFacade(isLeftRoom);
    drawRoomSpecifics();

    glPopMatrix();
}

void Room::drawGlassFacade(bool isLeftRoom) {
    const float DOOR_W = 1.8f, DOOR_H = 2.5f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);

    float glassWallX = isLeftRoom ? m_width : 0;
    float normalX = isLeftRoom ? 1.0f : -1.0f;

    float slideAmount = doorPosition * (DOOR_W * 0.6f);
    float doorZ1 = (m_depth / 2.0f) - (DOOR_W / 2.0f) - slideAmount;
    float doorZ2 = (m_depth / 2.0f) + (DOOR_W / 2.0f) - slideAmount;

    glBindTexture(GL_TEXTURE_2D, glassTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glBegin(GL_QUADS);
    glNormal3f(normalX, 0, 0);

    glTexCoord2f(0, 0); glVertex3f(glassWallX, 0, 0);
    glTexCoord2f(doorZ1 / m_depth, 0); glVertex3f(glassWallX, 0, doorZ1);
    glTexCoord2f(doorZ1 / m_depth, 0.5); glVertex3f(glassWallX, DOOR_H, doorZ1);
    glTexCoord2f(0, 0.5); glVertex3f(glassWallX, DOOR_H, 0);

    glTexCoord2f(doorZ2 / m_depth, 0); glVertex3f(glassWallX, 0, doorZ2);
    glTexCoord2f(1.0, 0); glVertex3f(glassWallX, 0, m_depth);
    glTexCoord2f(1.0, 0.5); glVertex3f(glassWallX, DOOR_H, m_depth);
    glTexCoord2f(doorZ2 / m_depth, 0.5); glVertex3f(glassWallX, DOOR_H, doorZ2);

    glTexCoord2f(0, 0.5); glVertex3f(glassWallX, DOOR_H, 0);
    glTexCoord2f(1.0, 0.5); glVertex3f(glassWallX, DOOR_H, m_depth);
    glTexCoord2f(1.0, 1.0); glVertex3f(glassWallX, m_height, m_depth);
    glTexCoord2f(0, 1.0); glVertex3f(glassWallX, m_height, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, doorGlassTexture);
    glColor4f(1, 1, 1, 0.6);
    glBegin(GL_QUADS);
    glNormal3f(normalX, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(glassWallX, 0, doorZ1);
    glTexCoord2f(1, 0); glVertex3f(glassWallX, 0, doorZ2);
    glTexCoord2f(1, 1); glVertex3f(glassWallX, DOOR_H, doorZ2);
    glTexCoord2f(0, 1); glVertex3f(glassWallX, DOOR_H, doorZ1);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glLineWidth(4.0f);
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(glassWallX, 0, doorZ1);
    glVertex3f(glassWallX, 0, doorZ2);
    glVertex3f(glassWallX, DOOR_H, doorZ2);
    glVertex3f(glassWallX, DOOR_H, doorZ1);
    glEnd();

    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

void Room::drawPlatform() {
    const float PLATFORM_RADIUS = 1.5f;
    const float PLATFORM_HEIGHT = 0.2f;

    GLfloat platformColor[4];
    getPlatformColor(platformColor);

    glMaterialfv(GL_FRONT, GL_AMBIENT, platformColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, platformColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, platformColor);

    glPushMatrix();
    glTranslatef(m_width / 2, PLATFORM_HEIGHT / 2, m_depth / 2);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluCylinder(quadric, PLATFORM_RADIUS, PLATFORM_RADIUS, PLATFORM_HEIGHT, 32, 32);
    gluDeleteQuadric(quadric);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(m_width / 2, PLATFORM_HEIGHT, m_depth / 2);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    quadric = gluNewQuadric();
    gluDisk(quadric, 0, PLATFORM_RADIUS, 32, 32);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void Room::update(float deltaTime) {
    carRotation += rotationSpeed * deltaTime;
    if (carRotation > 360.0f) {
        carRotation -= 360.0f;
    }
}

void Room::drawNeonCeiling() {
    setNeonColors();

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    const float neonWidth = 0.1f;
    const float neonGap = 0.3f;
    const float neonOffset = 0.2f;

    for (float x = neonOffset; x < m_width - neonOffset; x += neonGap) {
        glColor4f(neonColor1[0], neonColor1[1], neonColor1[2], 0.8f);
        glBegin(GL_QUADS);
        glVertex3f(x, m_height - 0.01f, neonOffset);
        glVertex3f(x + neonWidth, m_height - 0.01f, neonOffset);
        glVertex3f(x + neonWidth, m_height - 0.01f, m_depth - neonOffset);
        glVertex3f(x, m_height - 0.01f, m_depth - neonOffset);
        glEnd();

        glColor4f(neonColor2[0], neonColor2[1], neonColor2[2], 0.8f);
        glBegin(GL_QUADS);
        glVertex3f(x + neonWidth / 2, m_height - 0.01f, neonOffset);
        glVertex3f(x + neonWidth * 1.5f, m_height - 0.01f, neonOffset);
        glVertex3f(x + neonWidth * 1.5f, m_height - 0.01f, m_depth - neonOffset);
        glVertex3f(x + neonWidth / 2, m_height - 0.01f, m_depth - neonOffset);
        glEnd();
    }

    for (float z = neonOffset; z < m_depth - neonOffset; z += neonGap) {
        glColor4f(neonColor1[0], neonColor1[1], neonColor1[2], 0.8f);
        glBegin(GL_QUADS);
        glVertex3f(neonOffset, m_height - 0.01f, z);
        glVertex3f(m_width - neonOffset, m_height - 0.01f, z);
        glVertex3f(m_width - neonOffset, m_height - 0.01f, z + neonWidth);
        glVertex3f(neonOffset, m_height - 0.01f, z + neonWidth);
        glEnd();

        glColor4f(neonColor2[0], neonColor2[1], neonColor2[2], 0.8f);
        glBegin(GL_QUADS);
        glVertex3f(neonOffset, m_height - 0.01f, z + neonWidth / 2);
        glVertex3f(m_width - neonOffset, m_height - 0.01f, z + neonWidth / 2);
        glVertex3f(m_width - neonOffset, m_height - 0.01f, z + neonWidth * 1.5f);
        glVertex3f(neonOffset, m_height - 0.01f, z + neonWidth * 1.5f);
        glEnd();
    }

    for (int i = 0; i < 2; i++) {
        float glowSize = 0.05f * (i + 1);
        float glowAlpha = 0.3f / (i + 1);

        glColor4f(neonColor1[0], neonColor1[1], neonColor1[2], glowAlpha);
        for (float x = neonOffset; x < m_width - neonOffset; x += neonGap) {
            glBegin(GL_QUADS);
            glVertex3f(x - glowSize, m_height - 0.01f - glowSize / 2, neonOffset);
            glVertex3f(x + neonWidth + glowSize, m_height - 0.01f - glowSize / 2, neonOffset);
            glVertex3f(x + neonWidth + glowSize, m_height - 0.01f - glowSize / 2, m_depth - neonOffset);
            glVertex3f(x - glowSize, m_height - 0.01f - glowSize / 2, m_depth - neonOffset);
            glEnd();
        }

        glColor4f(neonColor2[0], neonColor2[1], neonColor2[2], glowAlpha);
        for (float z = neonOffset; z < m_depth - neonOffset; z += neonGap) {
            glBegin(GL_QUADS);
            glVertex3f(neonOffset, m_height - 0.01f - glowSize / 2, z - glowSize);
            glVertex3f(m_width - neonOffset, m_height - 0.01f - glowSize / 2, z - glowSize);
            glVertex3f(m_width - neonOffset, m_height - 0.01f - glowSize / 2, z + neonWidth + glowSize);
            glVertex3f(neonOffset, m_height - 0.01f - glowSize / 2, z + neonWidth + glowSize);
            glEnd();
        }
    }
    glPopAttrib();
}