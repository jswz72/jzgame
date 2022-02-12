#pragma once
#include <functional>
#include <iostream>


template <typename T=float>
class Vector2D {
public:
	T x;
	T y;

	Vector2D() : x(static_cast<T>(0)), y(static_cast<T>(0)) {};
	Vector2D(T x, T y) : x(x), y(y) {};

	friend Vector2D operator+(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return Vector2D(v1.x + v2.x, v1.y + v2.y);
	}
	friend Vector2D operator-(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return Vector2D(v1.x - v2.x, v1.y - v2.y);
	}
	friend Vector2D operator*(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return Vector2D(v1.x * v2.x, v1.y * v2.y);
	}
	friend Vector2D operator/(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return Vector2D(v1.x / v2.x, v1.y / v2.y);
	}

	friend Vector2D operator/(const Vector2D<T>& v1, const T& i) {
		return Vector2D(v1.x / i, v1.y / i);
	}

	bool operator==(const Vector2D& otherVec) const {
		return x == otherVec.x && y == otherVec.y;
	}

	Vector2D operator*(const T& i) {
		return Vector2D(x * i, y * i);
	}

	Vector2D& operator*=(const T& i) {
		x *= i;
		y *= i;
		return *this;
	}

	Vector2D& operator/=(const T& i) {
		x /= i;
		y /= i;
		return *this;
	}

	Vector2D& operator+=(const Vector2D<T>& vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}
	Vector2D& operator-=(const Vector2D<T>& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	Vector2D& operator*=(const Vector2D<T>& vec) {
		x *= vec.x;
		y *= vec.y;
		return *this;
	}
	Vector2D& operator/=(const Vector2D<T>& vec) {
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	struct HashFunction {
		size_t operator()(const Vector2D<T>& vec) const {
			size_t xHash = std::hash<T>()(vec.x);
			size_t yHash = std::hash<T>()(vec.y);
			return xHash ^ yHash;
		}
	};

	Vector2D& zero() {
		x = 0;
		y = 0;
		return *this;
	}

	T magnitude() const {
		return sqrt(x * x + y * y);
	}

	Vector2D normalized() const {
		if (magnitude() == 0) {
			return Vector2D<T>();
		}
		return { x / magnitude(), y / magnitude() };
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec) {
		stream << "(" << vec.x << "," << vec.y << ")";
		return stream;
	}
};
