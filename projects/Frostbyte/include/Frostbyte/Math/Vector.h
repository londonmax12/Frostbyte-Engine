#pragma once
#include <ostream>

namespace Frostbyte
{
    struct Vector2
    {
        Vector2(float X = 0.f, float Y = 0.f) 
            : x(X), y(Y) {}

        float x;
        float y;

        friend std::ostream& operator<<(std::ostream& os, const Vector2& vec)
        {
            os << "[" << vec.x << ", " << vec.y << "]";
            return os;
        }
    };
    struct VectorInt2
    {
        VectorInt2(int X = 0, int Y = 0) 
            : x(X), y(Y) {}

        int x;
        int y;

        friend std::ostream& operator<<(std::ostream& os, const VectorInt2& vec)
        {
            os << "[" << vec.x << ", " << vec.y << "]";
            return os;
        }
    };
    struct Vector3
    {
        Vector3(float X = 0.f, float Y = 0.f, float Z = 0.f) 
            : x(X), y(Y), z(Z) {}

        float x;
        float y;
        float z;

        friend std::ostream& operator<<(std::ostream& os, const Vector3& vec)
        {
            os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
            return os;
        }
    };
    struct VectorInt3
    {
        VectorInt3(int X = 0, int Y = 0, int Z = 0) 
            : x(X), y(Y), z(Z) {}

        int x;
        int y;
        int z;

        friend std::ostream& operator<<(std::ostream& os, const VectorInt3& vec)
        {
            os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
            return os;
        }
    };
}