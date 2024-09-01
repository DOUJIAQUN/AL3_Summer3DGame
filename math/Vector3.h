#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

//运算符重载
inline Vector3 operator+(const Vector3& leftVector, const Vector3& rightVector) { return Vector3(leftVector.x + rightVector.x, leftVector.y + rightVector.y, leftVector.z + rightVector.z); }

inline Vector3 operator-(const Vector3& leftVector, const Vector3& rightVector) { return Vector3(leftVector.x - rightVector.x, leftVector.y - rightVector.y, leftVector.z - rightVector.z); }

inline Vector3& operator+=(Vector3& leftVector, const Vector3& rightVector) {
	leftVector.x += rightVector.x;
	leftVector.y += rightVector.y;
	leftVector.z += rightVector.z;
	return leftVector;
}

inline Vector3& operator-=(Vector3& leftVector, const Vector3& rightVector) {
	leftVector.x -= rightVector.x;
	leftVector.y -= rightVector.y;
	leftVector.z -= rightVector.z;
	return leftVector;
}

inline Vector3 operator*(float Scaler, const Vector3& v) { return Vector3(Scaler * v.x, Scaler * v.y, Scaler * v.z); }