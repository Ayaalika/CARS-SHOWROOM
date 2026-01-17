#pragma once
#include <cmath>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

   Vector3 cross(const Vector3& other) const {
        return Vector3(y * other.z - z * other.y,
                       z * other.x - x * other.z,
                       x * other.y - y * other.x);
    }

    static float radians(float degrees) {
        constexpr float PI = 3.14159265358979323846f;
        return degrees * PI / 180.0f;
    }

    void normalize() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length > 0) {
            x /= length; y /= length; z /= length;
        }
    }
};