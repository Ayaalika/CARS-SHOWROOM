#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map> 
#include <future> 
#include <GL/freeglut.h> 
#include "../tiny_obj_loader.h"
#include "../Math/Vector3.h" 

struct DrawCall {
    GLuint textureID;
    int materialID;
    size_t startIndex;
    size_t count;
};

struct Material {
    std::string name;
    float diffuse[4];
    float ambient[4];
    float specular[4];
    float shininess;
    float alpha;
};

class CarModel {
public:
    CarModel(const std::string& modelPath);
    ~CarModel();

    void setAppropriateScale(float showroomScale);
    void load();

    void draw();
    void setPosition(const Vector3& position);
    void setRotation(float angle);
    void setScale(float scale);

    bool isModelLoaded() const { return isLoaded; }
    void setYScaleMultiplier(float multiplier);
private:
    std::string m_modelPath;
    Vector3 m_position;
    float m_rotation;
    float m_scale;
    bool isLoaded;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_texCoords;
    std::vector<GLuint> m_indices;

    std::vector<Material> m_materials; 
    std::vector<DrawCall> m_drawCalls; 

    std::map<std::string, GLuint> textures;
    std::future<bool> loadFuture;


    Vector3 m_minBounds, m_maxBounds;
    float m_yScaleMultiplier;

    bool loadModel(const std::string& filename);
    void setupMesh();
    GLuint loadTexture(const char* filepath);
    void calculateDimensions();
};