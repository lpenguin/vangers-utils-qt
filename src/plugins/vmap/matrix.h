#ifndef MATRIX_H
#define MATRIX_H
#include <assert.h>
#include <vector>

template<typename T>
class Matrix: public std::vector<T>{
public:
	Matrix(size_t sizeX, size_t sizeY)
		: std::vector<T>(sizeX * sizeY)
		, _sizeX(sizeX)
		, _sizeY(sizeY)
	{
		this->resize(sizeX * sizeY);
	}

	T& operator()(size_t x, size_t y){
		assert(x >= 0 && x < _sizeX);
		assert(y >= 0 && y < _sizeY);

		return this[x + y * _sizeX];
	}
private:
	size_t _sizeX;
	size_t _sizeY;
};
#endif // MATRIX_H
