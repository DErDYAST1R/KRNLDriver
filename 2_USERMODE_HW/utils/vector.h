#pragma once
#include <stdint.h>
#include <stdio.h>
#include <iostream>

struct view_matrix_t {
    float* operator[ ](int index) {
        return matrix[index];
    }

    float matrix[4][4];
};

struct Vector2 {
    float x, y;

    Vector2() : x(0), y(0) {}

    Vector2(float x_val, float y_val) : x(x_val), y(y_val) {}

    Vector2 Subtract(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 Add(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 Divide(const Vector2& other) const {
        return Vector2(x / other.x, y / other.y);
    }
    Vector2 Divide(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }
    Vector2 Multiply(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    Vector2 Lerp(const Vector2& target, float factor) const {
        return Vector2(x + factor * (target.x - x), y + factor * (target.y - y));
    }
    Vector2 Normalized() const {
        Vector2 result;
        float length = std::sqrt(x * x + y * y);
        if (length != 0) {
            result.x = x / length;
            result.y = y / length;
        }
        return result;
    }
    Vector2 MultipliedByScalar(float scalar) const {
        Vector2 result;
        result.x = x * scalar;
        result.y = y * scalar;
        return result;
    }
    Vector2 Clamp() const {
        float clampedX = x;
        if (clampedX < -89) clampedX = -89;
        if (clampedX > 89) clampedX = 89;
        float clampedY = y;
        if (clampedY < -180) clampedY += 360;
        if (clampedY > 180) clampedY -= 360;
        if (clampedX > 89 || clampedX < -89) throw std::invalid_argument("BAD PITCH CLAMPING. CHECK YOUR CODE");
        if (clampedY > 180 || clampedY < -180) throw std::invalid_argument("BAD YAW CLAMPING. CHECK YOUR CODE");
        return Vector2(clampedX, clampedY);
    }
    float DotProduct(const Vector2& other) const {
        return x * other.x + y * other.y;
    }
    float Magnitude() const {
        return std::sqrt(x * x + y * y);
    }
    float Distance(const Vector2& other) const {
        Vector2 diff = Subtract(other);
        return diff.Magnitude();
    }
    bool IsZeroVector() {
        return x == 0 && y == 0;
    }
    bool operator==(const Vector2& other) const {
        float epsilon = 1e-5;
        return (std::abs(x - other.x) < epsilon)
            && (std::abs(y - other.y) < epsilon);
    }
    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }
};

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(float x_val, float y_val, float z_val) : x(x_val), y(y_val), z(z_val) {}

    Vector3 Subtract(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 Add(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3& Normalize() {
        float len = Magnitude();
        if (len > 0) {
            x /= len;
            y /= len;
            z /= len;
        }
        return *this;
    }
    Vector3 Multiply(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector2 To2D() const {
        return Vector2(x, y);
    }
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    float DotProduct(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    float Magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    float Magnitude2D() const {
        return std::sqrt(x * x + y * y);
    }
    float Distance(const Vector3& other) const {
        Vector3 diff = Subtract(other);
        return diff.Magnitude();
    }
    float Distance2D(const Vector3& other) const {
        return (other.Subtract(*this)).Magnitude2D();
    };
    bool IsZeroVector() {
        return x == 0 && y == 0 && z == 0;
    }
    bool IsValid() {
        if (std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y) || std::isnan(z) || std::isinf(z)) {
            return false;
        }
        return true;
    }
    bool operator==(const Vector3& other) const {
        float epsilon = 1e-5;
        return (std::abs(x - other.x) < epsilon)
            && (std::abs(y - other.y) < epsilon)
            && (std::abs(z - other.z) < epsilon);
    }
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
};

struct Vector4 {
    float x, y, z, w;

    // Default constructor
    Vector4() : x(0), y(0), z(0), w(0) {}

    // Parameterized constructor
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Addition of two vectors
    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Subtraction of two vectors
    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Scalar multiplication
    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Dot product
    float dot(const Vector4& other) const {
        return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
    }

    // Magnitude of the vector
    float magnitude() const {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    // Normalize the vector
    Vector4 normalize() const {
        float mag = magnitude();
        return Vector4(x / mag, y / mag, z / mag, w / mag);
    }
};


int CalculateFontSize(float distance, float minSize = 8, float maxSize = 13, float referenceDistance = 100) {
    float size = referenceDistance / distance * maxSize;
    if (size < minSize) size = minSize;
    if (size > maxSize) size = maxSize;
    return static_cast<int>(size);
}