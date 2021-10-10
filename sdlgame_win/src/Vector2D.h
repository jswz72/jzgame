#pragma once
#include <iostream>

class Vector2D {
public:
	float x;
	float y;

	Vector2D() : x(0.0f), y(0.0f) {};
	Vector2D(float x, float y) : x(x), y(y) {};
	Vector2D(int xInt, int yInt) {
		x = static_cast<float>(xInt);
		y = static_cast<float>(yInt);
	}

	friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {
		return Vector2D(v1.x + v2.x, v1.y + v2.y);
	}
	friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {
		return Vector2D(v1.x - v2.x, v1.y - v2.y);
	}
	friend Vector2D operator*(const Vector2D& v1, const Vector2D& v2) {
		return Vector2D(v1.x * v2.x, v1.y * v2.y);
	}
	friend Vector2D operator/(const Vector2D& v1, const Vector2D& v2) {
		return Vector2D(v1.x / v2.x, v1.y / v2.y);
	}

	friend Vector2D operator/(const Vector2D& v1, const int& i) {
		return Vector2D(v1.x / i, v1.y / i);
	}

	friend Vector2D operator/(const Vector2D& v1, const float& i) {
		return Vector2D(v1.x / i, v1.y / i);
	}

	Vector2D operator*(const int& i) {
		return Vector2D(x * i, y * i);
	}

	Vector2D& operator*=(const int& i) {
		x *= i;
		y *= i;
		return *this;
	}

	Vector2D& operator*=(const float& i) {
		x *= i;
		y *= i;
		return *this;
	}

	Vector2D& operator/=(const float& i) {
		x /= i;
		y /= i;
		return *this;
	}

	Vector2D& operator+=(const Vector2D& vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}
	Vector2D& operator-=(const Vector2D& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	Vector2D& operator*=(const Vector2D& vec) {
		x *= vec.x;
		y *= vec.y;
		return *this;
	}
	Vector2D& operator/=(const Vector2D& vec) {
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	Vector2D& zero() {
		x = 0;
		y = 0;
		return *this;
	}

	float magnitude() const {
		return sqrt(x * x + y * y);
	}

	Vector2D normalized() const {
		if (magnitude() == 0) {
			return Vector2D();
		}
		return { x / magnitude(), y / magnitude() };
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec) {
		stream << "(" << vec.x << "," << vec.y << ")";
		return stream;
	}
};