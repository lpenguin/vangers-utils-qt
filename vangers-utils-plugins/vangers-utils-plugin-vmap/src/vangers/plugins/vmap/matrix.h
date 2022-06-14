#ifndef MATRIX_H
#define MATRIX_H
#include <assert.h>
#include <vector>

template<typename T>
class Matrix: public std::vector<T>{
public:
	Matrix(size_t sizeX = 0, size_t sizeY = 0)
		: std::vector<T>(sizeX * sizeY)
		, _sizeX(sizeX)
		, _sizeY(sizeY)
	{
		std::vector<T>::resize(sizeX * sizeY);
	}


	void setData(size_t x, size_t y, T data){
		assert(x >= 0 && x < _sizeX);
		assert(y >= 0 && y < _sizeY);
		std::vector<T>::at(x + y * _sizeX) = data;
	}

	const T getData(size_t x, size_t y) const{
		assert(x >= 0 && x < _sizeX);
		assert(y >= 0 && y < _sizeY);
		return std::vector<T>::at(x + y * _sizeX);
	}

	void resize(size_t sizeX, size_t sizeY){
		_sizeX = sizeX;
		_sizeY = sizeY;
		std::vector<T>::resize(sizeX * sizeY);
	}

	size_t sizeX() const {
		return _sizeX;
	}

	size_t sizeY() const {
		return _sizeY;
	}

private:
	size_t _sizeX;
	size_t _sizeY;
};
#endif // MATRIX_H
