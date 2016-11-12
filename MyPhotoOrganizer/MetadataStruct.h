#pragma once

struct Vector3D
{
	float x;
	float y;
	float z;
	explicit Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		: x(x), y(y), z(z) {};
};


//inline bool operator<(const Vector3D& lhs, const Vector3D& rhs)
//{
//	return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
//}

inline bool operator<(const Vector3D& lhs, const Vector3D& rhs)
{
	//return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
}