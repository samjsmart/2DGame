#pragma once
#include <math.h>
#include <string.h>

struct FVector3D;

struct FMatrix4x4 {
    float matrix[4][4] = { 0 };
};

struct FMatrix3x3 {
    float matrix[3][3] = { 0 };

    FMatrix3x3() {};
    FMatrix3x3(FMatrix3x3 const&) = default;
    FMatrix3x3(float newMatrix[3][3]) {
        memcpy(matrix, newMatrix, sizeof(matrix));
    }
    FMatrix3x3(
        float f1, float f2, float f3,
        float f4, float f5, float f6,
        float f7, float f8, float f9
    ) {
        matrix[0][0] = f1;
        matrix[0][1] = f2;
        matrix[0][2] = f3;

        matrix[1][0] = f4;
        matrix[1][1] = f5;
        matrix[1][2] = f6;

        matrix[2][0] = f7;
        matrix[2][1] = f8;
        matrix[2][2] = f9;
    }

    FMatrix3x3 operator*(const FMatrix3x3& rhs) {
        float newMatrix[3][3];

        newMatrix[0][0] = matrix[0][0] * rhs.matrix[0][0] + matrix[0][1] * rhs.matrix[1][0] + matrix[0][2] * rhs.matrix[2][0];
        newMatrix[0][1] = matrix[0][0] * rhs.matrix[0][1] + matrix[0][1] * rhs.matrix[1][1] + matrix[0][2] * rhs.matrix[2][1];
        newMatrix[0][2] = matrix[0][0] * rhs.matrix[0][2] + matrix[0][1] * rhs.matrix[1][2] + matrix[0][2] * rhs.matrix[2][2];

        newMatrix[1][0] = matrix[1][0] * rhs.matrix[0][0] + matrix[1][1] * rhs.matrix[1][0] + matrix[1][2] * rhs.matrix[2][0];
        newMatrix[1][1] = matrix[1][0] * rhs.matrix[0][1] + matrix[1][1] * rhs.matrix[1][1] + matrix[1][2] * rhs.matrix[2][1];
        newMatrix[1][2] = matrix[1][0] * rhs.matrix[0][2] + matrix[1][1] * rhs.matrix[1][2] + matrix[1][2] * rhs.matrix[2][2];

        newMatrix[2][0] = matrix[2][0] * rhs.matrix[0][0] + matrix[2][1] * rhs.matrix[1][0] + matrix[2][2] * rhs.matrix[2][0];
        newMatrix[2][1] = matrix[2][0] * rhs.matrix[0][1] + matrix[2][1] * rhs.matrix[1][1] + matrix[2][2] * rhs.matrix[2][1];
        newMatrix[2][2] = matrix[2][0] * rhs.matrix[0][2] + matrix[2][1] * rhs.matrix[1][2] + matrix[2][2] * rhs.matrix[2][2];

        return FMatrix3x3(newMatrix);
    }
};

struct FRotator {
    friend FVector3D;

private:
    float pitch, yaw, roll;
    FMatrix3x3 mRotation = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

public:
    FRotator() : pitch(0), yaw(0), roll(0) {}

    FRotator(float fPitch, float fYaw, float fRoll) {
        this->pitch = fPitch;
        this->yaw   = fYaw;
        this->roll  = fRoll;

        if (fPitch) {
            FMatrix3x3 mYRotate = {
                cosf(fPitch),  0, sinf(fPitch),
                0,             1, 0,
                -sinf(fPitch), 0, cosf(fPitch)
            };

            mRotation = mRotation * mYRotate;
        }

        if (fYaw) {
            FMatrix3x3 mZRotate = {
                cosf(fYaw), -sinf(fYaw), 0,
                sinf(fYaw), cosf(fYaw),  0,
                0,          0,           1
            };

            mRotation = mRotation * mZRotate;
        }

        if (fRoll) {
            FMatrix3x3 mXRotate = {
                1, 0,           0,
                0, cosf(fRoll), -sinf(fRoll),
                0, sinf(fRoll), cosf(fRoll)
            };

            mRotation = mRotation * mXRotate;
        }
    }
};

struct FVector2D {
    float X, Y;

    FVector2D() : X(0), Y(0) {}
    FVector2D(FVector2D const&) = default;
    FVector2D(float x, float y) : X(x), Y(y) {}

    FVector2D operator-(const FVector2D& rhs) {
        return FVector2D(X - rhs.X, Y - rhs.Y);
    }

    FVector2D operator-=(const FVector2D& rhs) {
        X -= rhs.X;
        Y -= rhs.Y;

        return *this;
    }

    FVector2D operator+(const FVector2D& rhs) {
        return FVector2D(X + rhs.X, Y + rhs.Y);
    }

    FVector2D operator+=(const FVector2D& rhs) {
        X += rhs.X;
        Y += rhs.Y;

        return *this;
    }

    FVector2D operator*(const float& rhs) {
        return FVector2D(X * rhs, Y * rhs);
    }
};

struct FRotator2D {
    float angle;

    FRotator2D() : angle(0) {}
    FRotator2D(float angle) : angle(angle) {}

    FVector2D operator*(const float& rhs) {
        return FVector2D((float)cos(angle) * rhs, (float)sin(angle) * rhs);
    }
};

struct FVector3D {
    float X, Y, Z;
    
    FVector3D() : X(0), Y(0), Z(0) {}
    FVector3D(FVector3D const&) = default;
    FVector3D(float x, float y, float z) : X(x), Y(y), Z(z) {}

    float dotProduct(FVector3D rhs) {
        FVector3D vMultiplied = *this * rhs;
        return vMultiplied.X + vMultiplied.Y + vMultiplied.Z;
    }

    FVector3D operator+(const FVector3D& rhs) {
        return FVector3D(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
    }

    FVector3D& operator+=(const FVector3D& rhs) {
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;

        return *this;
    }

    FVector3D operator-(const FVector3D& rhs) {
        return FVector3D(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
    }

    FVector3D& operator-=(const FVector3D& rhs) {
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;

        return *this;
    }
    
    FVector3D operator/(const float& rhs) {
        return FVector3D(X / rhs, Y / rhs, Z / rhs);
    }

    FVector3D& operator/=(const float& rhs) {
        X /= rhs;
        Y /= rhs;
        Z /= rhs;

        return *this;
    }

    FVector3D operator*(const FVector3D& rhs) {
        return FVector3D(
            X * rhs.X,
            Y * rhs.Y,
            Z * rhs.Z);
    }

    FVector3D& operator*=(const FVector3D& rhs) {
        FVector3D newVector = *this * rhs;

        X = newVector.X;
        Y = newVector.Y;
        Z = newVector.Z;

        return *this;
    }

    FVector3D operator*(const FMatrix3x3& rhs) {
        float newX = X * rhs.matrix[0][0] + Y * rhs.matrix[1][0] + Z * rhs.matrix[2][0];
        float newY = X * rhs.matrix[0][1] + Y * rhs.matrix[1][1] + Z * rhs.matrix[2][1];
        float newZ = X * rhs.matrix[0][2] + Y * rhs.matrix[1][2] + Z * rhs.matrix[2][2];

        return FVector3D(newX, newY, newZ);
    }

    FVector3D& operator*=(const FMatrix3x3& rhs) {
        FVector3D newVector = *this * rhs;

        X = newVector.X;
        Y = newVector.Y;
        Z = newVector.Z;

        return *this;
    }
    
    FVector3D operator*(const FRotator& rhs) {
        return *this * rhs.mRotation;
    }

    FVector3D& operator*=(const FRotator& rhs) {
        return *this *= rhs.mRotation;
    }
};

struct FVector4D {
    float X, Y, Z, W;

    FVector4D() : X(0), Y(0), Z(0), W(0) {}
    FVector4D(FVector4D const&) = default;
    FVector4D(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    FVector4D(const FVector3D& rhs) : X(rhs.X), Y(rhs.Y), Z(rhs.Z), W(0) {} 

    FVector4D& operator=(const FVector3D& rhs) {
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
        W = 0;

        return *this;
    }

    FVector4D operator*(const FMatrix4x4& rhs) {
        float newX = X * rhs.matrix[0][0] + Y * rhs.matrix[1][0] + Z * rhs.matrix[2][0] + W * rhs.matrix[3][0];
        float newY = X * rhs.matrix[0][1] + Y * rhs.matrix[1][1] + Z * rhs.matrix[2][1] + W * rhs.matrix[3][1];
        float newZ = X * rhs.matrix[0][2] + Y * rhs.matrix[1][2] + Z * rhs.matrix[2][2] + W * rhs.matrix[3][2];
        float newW = X * rhs.matrix[0][3] + Y * rhs.matrix[1][3] + Z * rhs.matrix[2][3] + W * rhs.matrix[3][3];

        return FVector4D(newX, newY, newZ, newW);
    }
};
