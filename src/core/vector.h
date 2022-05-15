#ifndef VECTOR_H
#define VECTOR_H

#include <QtMath>
#include <cstdint>

namespace vangers::core::vector {
	template<typename T>
	struct Vector3 {
		T x;
		T y;
		T z;

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

		template<typename K>
		operator Vector3<K>() const {
			return Vector3<K> {
				.x = (K)x,
				.y = (K)y,
				.z = (K)z,
			};
		}

		Vector3<T> normalized() const {
			double l = length();
			return {
				.x = (T)x / l,
				.y = (T)y / l,
				.z = (T)z / l,
			};
		}

		T length() const {
			return qSqrt(lengthSquared());
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
	};

}



#endif // VECTOR_H
