#ifndef VECTOR_H
#define VECTOR_H

#include <QtMath>
#include <cstdint>

namespace vangers::core::vector {
	template<typename T>
	struct Vector2 {
		T x;
		T y;

		T& operator[](const size_t i)
		{
			assert(i >= 0 && i < 2);
			return i<=0 ? x : y;
		}

		const T& operator[](const size_t i) const
		{
			assert(i >= 0 && i < 2);
			return i<=0 ? x : y;
		}
	};

	template<typename T>
	struct Vector3 {
		T x;
		T y;
		T z;

		template<typename K>
		static Vector3<T> fromVector(const Vector3<K>& v) {
			return Vector3<T> {
				.x = (T)v.x,
				.y = (T)v.y,
				.z = (T)v.z,
			};
		}

		template<typename K>
		static Vector3<T> round(const Vector3<K>& v) {
			return {
				.x = (T)::qRound(v.x),
				.y = (T)::qRound(v.y),
				.z = (T)::qRound(v.z),
			};
		}

		Vector3 operator-(const Vector3<T>& v) const{
			return {
				.x = x - v.x,
				.y = y - v.y,
				.z = z - v.z,
			};
		}

		Vector3 operator+(const Vector3<T>& v) const{
			return {
				.x = x + v.x,
				.y = y + v.y,
				.z = z + v.z,
			};
		}

		Vector3 operator*(const Vector3<T>& v) const{
			return {
				.x = x * v.x,
				.y = y * v.y,
				.z = z * v.z,
			};
		}

		Vector3 operator*(const T& v) const{
			return {
				.x = x * v,
				.y = y * v,
				.z = z * v,
			};
		}

		Vector3 operator/(const T& v) const{
			return {
				.x = x / v,
				.y = y / v,
				.z = z / v,
			};
		}

		Vector3 operator/(const Vector3<T>& v) const{
			return {
				.x = x / v.x,
				.y = y / v.y,
				.z = z / v.z,
			};
		}

		Vector3<T> normalized() const {
			double l = length();
			return {
				.x = (T)((double)x / l),
				.y = (T)((double)y / l),
				.z = (T)((double)z / l),
			};
		}

		T length() const {
			return (T)qSqrt(lengthSquared());
		}

		T lengthSquared() const {
			return (T)(this->dot(*this));
		}

		T dot(const Vector3& v) const {
			return x * v.x + y * v.y + z * v.z;
		}

		Vector3<T> cross(const Vector3<T> v) const {
			return {
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			};
		}

		T& operator[](const size_t i)
		{
			assert(i >= 0 && i < 3);
			return i<=0 ?
						x :
						i == 1 ?
							y :
							z;
		}

		const T& operator[](const size_t i) const
		{
			assert(i >= 0 && i < 3);
			return i<=0 ?
						x :
						i == 1 ?
							y :
							z;
		}
	};

	template<typename T>
	struct Matrix3x3 {
		T values[9];

		static Matrix3x3<T> identity(){
			return {
				1, 0, 0,
				0, 1, 0,
				0, 0, 1,
			};
		}
	};

	typedef Vector3<int8_t> Vector3I8;
	typedef Vector3<int32_t> Vector3I32;
	typedef Vector3<double> Vector3F64;
	typedef Vector3<float> Vector3F32;
	typedef Matrix3x3<double> Matrix3x3F64;

	template<typename T>
	struct Size3{
		Vector3<T> min;
		Vector3<T> max;

		Vector3<T> mid() const {
			return (max + min) / 2.0;
		}

		Vector3<T> size() const {
			return (max - min);
		}

		void move(const Vector3<T>& v) {
			min = min + v;
			max = max + v;
		}
	};

	typedef Size3<double> Size3F64;
}



#endif // VECTOR_H
