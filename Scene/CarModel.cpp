#include "CarModel.h"
#include <iostream>
#include <cmath>
#include <chrono>

#include "../stb_image.h" 

 #define TINYOBJLOADER_IMPLEMENTATION 
#include "../tiny_obj_loader.h"

CarModel::CarModel(const std::string& modelPath)
    : m_modelPath(modelPath), m_position(0.0f, 0.0f, 0.0f),
    m_rotation(0.0f), m_scale(1.0f), isLoaded(false) {
}

CarModel::~CarModel() {
    for (const auto& pair : textures) {
        glDeleteTextures(1, &pair.second);
    }
}

void CarModel::load() {
    if (isLoaded || loadFuture.valid()) {
        return;
    }
    loadFuture = std::async(std::launch::async, &CarModel::loadModel, this, m_modelPath);
}

void CarModel::draw() {
    if (loadFuture.valid()) {
        if (loadFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            if (loadFuture.get()) {
                setupMesh();
                isLoaded = true;
                std::cout << "Model loaded and ready for drawing with Vertex Arrays." << std::endl;
            }
            else {
                std::cerr << "Failed to load model data." << std::endl;
            }
        }
    }

    if (!isLoaded) {
        return;
    }

    if (m_vertices.empty() || m_indices.empty()) {
        return;
    }

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(m_rotation, 0.0f, 1.0f, 0.0f);
    glScalef(m_scale, m_scale, m_scale);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_vertices.data());

    if (!m_normals.empty()) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, m_normals.data());
    }
    else {
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if (!m_texCoords.empty()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords.data());
    }
    else {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    for (const auto& drawCall : m_drawCalls) {
        if (drawCall.textureID != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, drawCall.textureID);
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }

        const GLvoid* indexPointer = (const GLvoid*)(m_indices.data() + drawCall.startIndex);

        glDrawElements(GL_TRIANGLES,
            static_cast<GLsizei>(drawCall.count),
            GL_UNSIGNED_INT,
            indexPointer);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void CarModel::setPosition(const Vector3& position) { m_position = position; }
void CarModel::setRotation(float angle) { m_rotation = angle; }
void CarModel::setScale(float scale) { m_scale = scale; }

GLuint CarModel::loadTexture(const char* filepath) {
    if (textures.find(filepath) != textures.end()) {
        return textures[filepath];
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = GL_RGB;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 4) format = GL_RGBA;

    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    textures[filepath] = textureID;
    std::cout << "Successfully loaded texture: " << filepath << " with ID: " << textureID << std::endl;

    return textureID;
}

bool CarModel::loadModel(const std::string& filename) {
    std::string warn;
    std::string err;
    std::string base_dir = filename.substr(0, filename.find_last_of("/\\") + 1);

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), base_dir.c_str());

    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    if (!err.empty()) { std::cerr << "ERR: " << err << std::endl; return false; }
    if (!ret) return false;

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        int current_material_id = -1;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int material_id = shapes[s].mesh.material_ids[f];

            if (material_id != current_material_id) {
                current_material_id = material_id;
                DrawCall dc;
                dc.textureID = 0; 
                dc.startIndex = m_indices.size();
                dc.count = 0;
                m_drawCalls.push_back(dc);
            }

            for (size_t v = 0; v < 3; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                m_vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                m_vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                m_vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                if (idx.normal_index >= 0) {
                    m_normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    m_normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    m_normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
                }
                else {
                    m_normals.push_back(0); m_normals.push_back(0); m_normals.push_back(0);
                }
                if (idx.texcoord_index >= 0) {
                    m_texCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    m_texCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
                else {
                    m_texCoords.push_back(0); m_texCoords.push_back(0);
                }
                m_indices.push_back(m_indices.size());
            }
            if (!m_drawCalls.empty()) {
                m_drawCalls.back().count += 3;
            }
            index_offset += 3;
        }
    }

    return true;
}

void CarModel::setupMesh() {
    std::string base_dir = m_modelPath.substr(0, m_modelPath.find_last_of("/\\") + 1);

    size_t currentDrawCallIndex = 0;
    for (size_t s = 0; s < shapes.size(); s++) {
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            if (f == 0 || shapes[s].mesh.material_ids[f] != shapes[s].mesh.material_ids[f - 1]) {
                if (currentDrawCallIndex < m_drawCalls.size()) {
                    int material_id = shapes[s].mesh.material_ids[f];
                    if (material_id >= 0 && !materials[material_id].diffuse_texname.empty()) {
                        std::string texPath = base_dir + materials[material_id].diffuse_texname;
                        m_drawCalls[currentDrawCallIndex].textureID = loadTexture(texPath.c_str());
                    }
                    currentDrawCallIndex++;
                }
            }
        }
    }
}

void CarModel::setAppropriateScale(float showroomScale) {
    m_scale = 0.02f / showroomScale;
}