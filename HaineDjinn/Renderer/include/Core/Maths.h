#pragma once

#include <cmath>
#include <iostream>
#include <assimp/matrix4x4.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Constants.h"


namespace Maths
{
	// TYPES
	union Vector2
	{
		float e[2];
		struct { float x; float y; };
	};

	union int3
	{
		int e[3];
		struct { int v; int t; int n; };
	};

	union Vector3
	{
		float e[3];
		struct { float x; float y; float z; };
		struct { float r; float g; float b; };
	};

	union Vector4
	{
		Vector4() = default;
		Vector4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{}

		Vector4(Vector3 xyz, float w = 0)
			: x(xyz.x), y(xyz.y), z(xyz.z), w(w)
		{}

		float e[4];
		struct { float x; float y; float z; float w; };
		struct { float r; float g; float b; float a; };
		Vector3 xyz;
	};

	struct Quaternion
	{
		float x;
		float y;
		float z;
		float w; // scalar
	};

	union mat4x4
	{
		float e[16];
		Vector4 c[4];
	};

	class Plane
	{
	public:
		Plane() {};
		Plane(const Vector3 inNormal, const float inD) { normal = inNormal; d = inD; };
		Plane(const Vector3 inNormal, const Vector3 inPoint);
		Plane(const Vector3 p1, const Vector3 p2, const Vector3 p3);
		~Plane() {};

		Vector3 normal = {};
		float d = 0;
	};

	struct Sphere
	{
		Vector3 center;
		float radius;
	};

	struct Cylinder
	{
		Vector3 p1;
		Vector3 p2;
		float radius;
		bool infinite = false;
	};

	struct Capsule
	{
		Vector3 p1;
		Vector3 p2;
		float radius;
	};

	struct Quad
	{
		float width, length;
		Vector3 center;
		Vector3 rotation; // NOTE: This rotation is in radians
	};

	struct Box
	{
		Vector3 dimensions;
		Vector3 center;
		Vector3 rotation;
	};

	struct RoundedBox
	{
		Vector3 dimensions;
		Vector3 center;
		Vector3 rotation;
		float radius;
	};

	struct referential3D
	{
		Vector3 origin;
		Vector3 i;
		Vector3 j;
		Vector3 k;
		Vector3 rotate;
	};
}

// TODO: Perhaps remove completely this namespace (using namespace to avoid in .h)
using namespace Maths;

static mat4x4 operator*(const mat4x4& a, const mat4x4& b)
{
	mat4x4 retMat = {};

	for (int line = 0; line < 4; line++)
		for (int column = 0; column < 4; column++)
			for (int i = 0; i < 4; i++)
				retMat.c[column].e[line] += a.c[i].e[line] * b.c[column].e[i];

	return retMat;
}

namespace Maths
{


	// FUNCTIONS
	// VECTOR2 CALCULATIONS
	int compareVector2(const Vector2& vector1, const Vector2& vector2, float epsilon);
	float lengthVector2(const Vector2& vector);
	float squaredLengthVector2(const Vector2& vector);
	float pointsDistance(const Vector2& p1, const Vector2& p2);
	float dotProductVector2(const Vector2& vector1, const Vector2& vector2);
	float angleVector2(const Vector2& vector1, const Vector2& vector2);
	float angleDegVector2(const Vector2& vector1, const Vector2& vector2);

	// VECTOR2 MANIPULATION
	Vector2 additionVector2(const Vector2& vector1, const Vector2& vector2);
	Vector2 negateVector2(const Vector2& vector);
	Vector2 unitVector2(Vector2& vector);
	Vector2 substractVector2(const Vector2& vector1, const Vector2& vector2);
	Vector2 scaleVector2(const Vector2& vector, float scale);
	Vector2 normalVector2(const Vector2& vector);
	void vectRotate(Vector2& vector, float angle);
	void vectRotate90(Vector2& vector);
	void pointRotate(const Vector2& origin, Vector2& point, float angle);

	// VECTOR3 FUNCTIONS
	inline Vector3 GetVec3(aiVector3D& vec) { return Vector3{ vec.x, vec.y, vec.z }; }
	inline Vector3 nullVector3() { return { 0.f, 0.f, 0.f }; };
	inline float dotVector3(const Vector3& vec1, const Vector3& vec2) { return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z; };
	inline Vector3 crossProductVector3(const Vector3& vec1, const Vector3& vec2) { return { vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x }; };
	inline float lengthVector3(const Vector3& vector) { return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z); };
	inline float squaredLengthVector3(const Vector3 vector) { return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z; };
	inline Vector3 normalizeVector3(const Vector3 vector) { float len = lengthVector3(vector); return { vector.x / len, vector.y / len, vector.z / len }; };

	// INTERSECTION FUNCTIONS
	bool intersectSegmentSphere(const Vector3& p1, const Vector3& p2, const Sphere& sphere, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentPlane(const Vector3& p1, const Vector3& p2, const Plane& plane, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentQuad(const Vector3& p1, const Vector3& p2, const Quad& quad, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentCylinder(const Vector3& p1, const Vector3& p2, const Cylinder& cylinder, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentCylinderInfinite(const Vector3& p1, const Vector3& p2, const Cylinder& cylinder, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentCylinderFinite(const Vector3& p1, const Vector3& p2, const Cylinder& cylinder, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectLinePlane(const Vector3& p1, const Vector3& p2, const Plane& plane, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentCapsule(const Vector3& p1, const Vector3& p2, const Capsule& capsule, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentBox(const Vector3& p1, const Vector3& p2, const Box& box, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSegmentRoundedBox(const Vector3& p1, const Vector3& p2, const RoundedBox& box, Vector3& intersectPt, Vector3& intersectNormal);
	bool intersectSphereBox(const Vector3& velocity, const Sphere& sphere, const Box& box, Vector3& intersectPt, Vector3& intersectNormal);

	// COLLISION FUNCTIONS
	bool collisionSphereBox(const Sphere& sphere, const Box& box);
	bool collisionBoxBox(const Box& box1, const Box& box2);
	bool collisionSphereSphere(const Sphere& sphere1, const Sphere& sphere2);

	// QUATERNION FUNCTIONS
	Vector3 RotateVectorByQuaternion(Vector3 v, Quaternion q);
	Vector3 QuaternionToEulerRadians(Quaternion q);
	Quaternion EulerToQuaternion(Vector3 eulerAngles);
	float QuaternionLength(Quaternion q);
	inline Quaternion GetQuaternionFromAssimp(const aiQuaternion& quat) { return Quaternion{ quat.x, quat.y, quat.z, quat.w }; };
	inline Quaternion QuaternionIdentity() { return Quaternion{ 0.f, 0.f, 0.f, 1.f }; };
	Quaternion QuaternionFromEuler(float roll, float pitch, float yaw);
	Quaternion QuaternionFromEuler(Vector3 euler);
	Quaternion QuaternionMultiply(Quaternion q1, Quaternion q2);
	Quaternion QuaternionNormalize(Quaternion q);
	Quaternion QuaternionFromAxisAngle(Vector3 axis, float angle);

	// OTHER FUNCTIONS
	float getFloatsMin4(float num1, float num2, float num3, float num4);
	float getFloatsMin4(Vector4 vec);
	float getFloatsMax4(float num1, float num2, float num3, float num4);

	inline int maxValue(const int a, const int b)
	{
		return a > b ? a : b;
	}

	inline int minValue(const int a, const int b)
	{
		return a < b ? a : b;
	}

	float clamp(float toClamp, float min, float max);
	int clamp(int toClamp, int min, int max);

	int remap(int val, int min1, int max1, int min2, int max2);

	float lerp(const float& from, const float& to, const float& t);
	Vector3 lerp(const Vector3& from, const Vector3& to, const float& t);
	Quaternion nlerp(const Quaternion& from, const Quaternion& to, const float& t);
	Quaternion Slerp(const Quaternion& from, const Quaternion& to, const float& t);

	namespace mat4
	{
		// MAT4 FUNCTIONS
		Vector3 RotationMatToEuler(const mat4x4& mat);
		Quaternion RotationMatToQuaternion(const mat4x4& mat);
		mat4x4 Invert(mat4x4& mat);
		mat4x4 perspective(float fovY, float aspect, float near, float far);
		mat4x4 orthographic(float fovY, float aspect, float near, float far);
		mat4x4 frustum(float left, float right, float bottom, float top, float near, float far);

		inline mat4x4 Identity()
		{
			return {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f,
			};
		}

		inline Maths::mat4x4 RotateX(float angleRadians)
		{
			return {
				1,                   0,                  0,                 0,
				0,                   cos(angleRadians),  sin(angleRadians), 0,
				0,                   -sin(angleRadians), cos(angleRadians), 0,
				0,                   0,                  0,                 1
			};
		}

		inline Maths::mat4x4 RotateY(float angleRadians)
		{
			return {
				cos(angleRadians), 0, sin(angleRadians), 0,
				0, 1,  0, 0,
				-sin(angleRadians), 0, cos(angleRadians), 0,
				0, 0, 0, 1
			};
		}

		inline Maths::mat4x4 RotateZ(float angleRadians)
		{
			return {
				cos(angleRadians),  sin(angleRadians),   0,                 0,
				-sin(angleRadians), cos(angleRadians),   0,					0,
				0,                   0,					 1,					0,
				0,                   0,                  0,                 1
			};
		}

		// TODO: Remove this
		//static mat4x4 operator*(const mat4x4 a, const mat4x4 b)
		//{
		//	mat4x4 retMat = {};
		//
		//	for (int line = 0; line < 4; line++)
		//		for (int column = 0; column < 4; column++)
		//			for (int i = 0; i < 4; i++)
		//				retMat.c[column].e[line] += a.c[i].e[line] * b.c[column].e[i];
		//
		//	return retMat;
		//}

		inline mat4x4 RotateQuat(Quaternion q)
		{
#ifdef HORIZONTAL_MATRIX
			return {
				1.f - 2.f * (q.y * q.y + q.z * q.z),	2.f * (q.x * q.y + q.w * q.z),			2.f * (q.x * q.z - q.w * q.y),		 0.f,
				2.f * (q.x * q.y - q.w * q.z),			1.f - 2.f * (q.x * q.x + q.z * q.z),	2.f * (q.y * q.z + q.w * q.x),		 0.f,
				2.f * (q.x * q.z + q.w * q.y),			2.f * (q.y * q.z - q.w * q.x),			1.f - 2.f * (q.x * q.x + q.y * q.y), 0.f,
				0.f,										0.f,										0.f,									 1.f
			};
#else
			return {
				2.f * (q.w * q.w + q.x + q.x) - 1.f,	2.f * (q.x * q.y + q.w * q.z),			2.f * (q.x * q.z - q.w * q.y),		 0,
				2.f * (q.x * q.y - q.w * q.z),			2.f * (q.w * q.w + q.y + q.y) - 1.f,	2.f * (q.y * q.z + q.w * q.x),		 0,
				2.f * (q.x * q.z + q.w * q.y),			2.f * (q.y * q.z - q.w * q.x),			2.f * (q.w * q.w + q.z + q.z) - 1.f, 0,
				0,										0,										0,									 0
			};
#endif
		}

		inline Maths::mat4x4 Rotate3f(const Vector3 rotation)
		{
			// Reduce computation
			return RotateY(rotation.y) * RotateX(rotation.x) * RotateZ(rotation.z);
		}

		inline Maths::mat4x4 Scale(float scale)
		{
			if (scale == 0)
				scale = 0.0001f;

			return {
				scale, 0.f, 0.f, 0.f,
				0.f, scale, 0.f, 0.f,
				0.f, 0.f, scale, 0.f,
				0.0f, 0.0f, 0.f, 1.f,
			};
		}

		inline Maths::mat4x4 Scale(Maths::Vector3 scale)
		{
			if (Maths::lengthVector3(scale) == 0)
				scale.x = 0.0001f;

			return {
				scale.x, 0.f,     0.f,     0.f,
				0.f,     scale.y, 0.f,     0.f,
				0.f,     0.f,     scale.z, 0.f,
				0.0f,    0.0f,    0.f,     1.f,
			};
		}

		inline Maths::mat4x4 Translate(Vector3 translate)
		{
#if HORIZONTAL_MATRIX
			return {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate.x, translate.y, translate.z, 1
			};
#else
			return {
				1, 0, 0, translate.x,
				0, 1, 0, translate.y,
				0, 0, 1, translate.z,
				0, 0, 0, 1
			};
#endif
		}
	}
}

// MATHS OPERATORS
static Vector4 operator*(const mat4x4& m, Vector4 v)
{
	Vector4 retFloat = {};

	for (int i = 0; i < 4; i++)
		retFloat.e[i] = v.e[0] * m.c[0].e[i] + v.e[1] * m.c[1].e[i] + v.e[2] * m.c[2].e[i] + v.e[3] * m.c[3].e[i];

	return retFloat;
}

inline Vector3 operator/(Vector3 v, float a) { return { v.x / a, v.y / a, v.z / a }; };
inline Vector3 operator*(Vector3 v, float a) { return { v.x * a, v.y * a, v.z * a }; };
inline Vector3 operator/(float a, Vector3 v) { return { v.x / a, v.y / a, v.z / a }; };
inline Vector3 operator*(float a, Vector3 v) { return { v.x * a, v.y * a, v.z * a }; };

inline Vector3 operator-(Vector3 v) { return { -v.x, -v.y, -v.z }; };

inline Vector3 operator-(Vector3 v1, Vector3 v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; };
inline Vector3 operator*(Vector3 v1, Vector3 v2) { return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z }; };
inline Vector3 operator+(Vector3 v1, Vector3 v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; };
inline Vector3 operator+=(Vector3 v1, Vector3 v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; };
inline Vector3 operator-=(Vector3 v1, Vector3 v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; };

inline Vector4 operator*(Vector4 v, float a) { return { v.x * a, v.y * a, v.z * a, v.w * a }; };
inline Vector4 operator+(Vector4 a, Vector4 b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; };

inline Quaternion operator*(Quaternion q, float a) { return { q.x * a, q.y * a, q.z * a, q.w * a }; };
inline Quaternion operator*(Quaternion q1, Quaternion q2) 
{ 
	Quaternion result = { 0 };

	result.x = q1.x * q2.w + q1.w * q2.x + q1.y * q2.z - q1.z * q2.y;
	result.y = q1.y * q2.w + q1.w * q2.y + q1.z * q2.x - q1.x * q2.z;
	result.z = q1.z * q2.w + q1.w * q2.z + q1.x * q2.y - q1.y * q2.x;
	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

	return result;
};
inline Quaternion operator+(Quaternion a, Quaternion b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; };

inline bool operator==(Vector3 v1, Vector3 v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
		return true;
	else
		return false;
}
inline bool operator!=(Vector3 v1, Vector3 v2)
{
	if (v1.x != v2.x && v1.y != v2.y && v1.z != v2.z)
		return true;
	else
		return false;
}

inline std::ostream& operator<<(std::ostream& os, Vector3 vector) { os << '{' << vector.x << ", " << vector.y << ", " << vector.z << '}'; return os; };
inline std::ostream& operator<<(std::ostream& os, Vector4 vector) { os << '{' << vector.x << ", " << vector.y << ", " << vector.z << vector.w << '}'; return os; };
