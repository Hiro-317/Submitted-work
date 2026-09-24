#pragma once

#include<cmath>

#include"Vector3.h"

struct Vector2;

struct Vector2I
{
	int x, y;

#pragma region ÉRÉìÉXÉgÉâÉNÉ^
	// Vector2Iê∂ê¨
	Vector2I(void) :x(0), y(0) {}

	// Vector2Iê∂ê¨
	Vector2I(int x, int y) :x(x), y(y) {}

	// Vector2Iê∂ê¨
	Vector2I(int value) :x(value), y(value) {}

	// Vector2Iê∂ê¨
	Vector2I(float x, float y) :x((int)x), y((int)y) {}

	// Vector2Iê∂ê¨
	Vector2I(float value) :x((int)value), y((int)value) {}
#pragma endregion


#pragma region Vector2Iê∂ê¨(ì¡éÍ)
	// XÇÃÇ›
	static Vector2I Xonly(int value) { return Vector2I(value, 0); }

	// YÇÃÇ›
	static Vector2I Yonly(int value) { return Vector2I(0, value); }
#pragma endregion

	// ë„ì¸
	void operator=(const int value) { x = value; y = value; }
	void operator=(const Vector2& value);

#pragma region ââéZ
	// â¡éZ---------------------------------------
	Vector2I operator+(const Vector2I value)const { return Vector2I(x + value.x, y + value.y); }
	void operator+=(const Vector2I value) { x = x + value.x; y = y + value.y; }

	Vector2I operator+(const int value)const { return Vector2I(x + value, y + value); }
	void operator+=(const int value) { x += value; y += value; }
	//--------------------------------------------
	
	// å∏éZ---------------------------------------
	Vector2I operator-(const Vector2I value)const { return Vector2I(x - value.x, y - value.y); }
	void operator-=(const Vector2I value) { x = x - value.x; y = y - value.y; }

	Vector2I operator-(const int value)const { return Vector2I(x - value, y - value); }
	void operator-=(const int value) { x -= value; y -= value; }
	//--------------------------------------------

	// èÊéZ---------------------------------------
	Vector2I operator*(const Vector2I value)const { return Vector2I(x * value.x, y * value.y); }
	void operator*=(const Vector2I value) { x *= value.x; y *= value.y; }

	Vector2I operator*(const int value)const { return Vector2I(x * value, y * value); }
	void operator*=(const int value) { x = x * value; y = y * value; }
	//--------------------------------------------

	// èúéZ---------------------------------------
	Vector2I operator/(const Vector2I value)const { return Vector2I(x / value.x, y / value.y); }
	void operator/=(const Vector2I value) { x /= value.x; y /= value.y; }

	Vector2I operator/(const int value)const { return Vector2I(x / value, y / value); }

	Vector2 operator/(const float value)const;
	void operator/=(const int value) { x = x / value; y = y / value; }
	//--------------------------------------------

#pragma endregion

	bool operator==(const Vector2I value)const { return (x == value.x && y == value.y); }
	bool operator==(const int value)const { return (x == value && y == value); }
	bool operator!=(const Vector2I value)const { return (x != value.x || y != value.y); }
	bool operator!=(const int value)const { return (x != value || y != value); }

	Vector2 ToVector2(void)const;

	float LengthSq(void)const { return (float)(x * x + y * y); }
	float Length(void)const { return sqrtf((float)(x * x + y * y)); }

	Vector2 Normalized(void)const;
};

struct Vector2
{
	float x, y;

#pragma region ÉRÉìÉXÉgÉâÉNÉ^
	// Vector2ê∂ê¨
	Vector2(void) :x(0.0f), y(0.0f) {}

	// Vector2ê∂ê¨
	Vector2(float x, float y) :x(x), y(y) {}

	// Vector2ê∂ê¨
	Vector2(float value) :x(value), y(value) {}

	// Vector2ê∂ê¨
	Vector2(int x, int y) :x((float)x), y((float)y) {}

	// Vector2ê∂ê¨
	Vector2(int value) :x((float)value), y((float)value) {}

	Vector2(const Vector2I& value) :x((float)value.x), y((float)value.y) {}
#pragma endregion


#pragma region Vector2ê∂ê¨(ì¡éÍ)
	// XÇÃÇ›
	static Vector2 Xonly(float value) { return Vector2(value, 0.0f); }

	// YÇÃÇ›
	static Vector2 Yonly(float value) { return Vector2(0.0f, value); }
#pragma endregion

	// ë„ì¸
	void operator=(const float value) { x = value; y = value; }

#pragma region ââéZ
	// â¡éZ---------------------------------------
	Vector2 operator+(const Vector2 value)const { return Vector2(x + value.x, y + value.y); }
	void operator+=(const Vector2 value) { x = x + value.x; y = y + value.y; }

	Vector2 operator+(float value)const { return Vector2(x + value, y + value); }
	void operator+=(float value) { x += value; y += value; }

	Vector2 operator+(int value)const { return Vector2(x + (float)value, y + (float)value); }
	void operator+=(int value) { x += (float)value; y += (float)value; }
	//--------------------------------------------

	// å∏éZ---------------------------------------
	Vector2 operator-(const Vector2 value)const { return Vector2(x - value.x, y - value.y); }
	void operator-=(const Vector2 value) { x = x - value.x; y = y - value.y; }

	Vector2 operator-(float value)const { return Vector2(x - value, y - value); }
	void operator-=(float value) { x -= value; y -= value; }

	Vector2 operator-(int value)const { return Vector2(x - (float)value, y - (float)value); }
	void operator-=(int value) { x -= (float)value; y -= (float)value; }
	//--------------------------------------------

	// èÊéZ---------------------------------------
	Vector2 operator*(const Vector2 value)const { return Vector2(x * value.x, y * value.y); }
	void operator*=(const Vector2 value) { x *= value.x; y *= value.y; }

	Vector2 operator*(float value)const { return Vector2(x * value, y * value); }
	void operator*=(float value) { x = x * value; y = y * value; }

	Vector2 operator*(int value)const { return Vector2(x * (float)value, y * (float)value); }
	void operator*=(int value) { x *= (float)value; y *= (float)value; }
	//--------------------------------------------

	// èúéZ---------------------------------------
	Vector2 operator/(const Vector2 value)const { return Vector2(x / value.x, y / value.y); }
	void operator/=(const Vector2 value) { x /= value.x; y /= value.y; }

	Vector2 operator/(const int value)const { return Vector2(x / value, y / value); }

	Vector2 operator/(const float value)const { return Vector2(x / value, y / value); }
	void operator/=(const int value) { x = x / value; y = y / value; }
	//--------------------------------------------
#pragma endregion

	bool operator==(const Vector2 value)const { return (x == value.x && y == value.y); }
	bool operator==(const float value)const { return (x == value && y == value); }
	bool operator!=(const Vector2 value)const { return (x != value.x	 || y != value.y); }
	bool operator!=(const float value)const { return  (x != value || y != value); }

	Vector2I ToVector2I(void)const { return Vector2I(x, y); }

	// XZïΩñ Ç…Ç®ÇØÇÈà⁄ìÆÉxÉNÉgÉãÇ»Ç«Ç…égÇ§ïœä∑
	Vector3 ToVector3XZ(void)const { return Vector3::XZonly(x, -y); }

	// YXÇÃÉJÉÅÉââÒì]Ç»Ç«Ç…égÇ§ïœä∑
	Vector3 ToVector3YX(void)const { return Vector3::XYonly(y, x); }

	float LengthSq(void)const { return (x * x + y * y); }
	float Length(void)const { return sqrtf(x * x + y * y); }

	Vector2 Normalized(void)const { return Vector2(x, y) / Length(); }
	void Normalize(void) { float len = Length(); x /= len; y /= len; }

	// çsóÒÇ≈ïœä∑
	Vector2 TransMat(float rot) const
	{
		if (*this == 0.0f) { return Vector2(); }

		float c = cosf(rot);
		float s = sinf(rot);

		return Vector2(
			x * c - y * s,
			x * s + y * c
		);
	}

	// çsóÒÇ≈ïœä∑(é©êgÇïœä∑)
	void TransMatOwn(float rot)
	{
		if (*this == 0.0f) { return; }

		float c = cosf(rot);
		float s = sinf(rot);

		float nx = x * c - y * s;
		float ny = x * s + y * c;

		x = nx;
		y = ny;
	}
};