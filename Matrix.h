#pragma once

#include <algorithm>
#include <vector>
#include <istream>
#include <ostream>

using std::vector;

template<typename T>
class Matrix
{
public:
	Matrix();
	Matrix(size_t, size_t);
	Matrix(size_t, size_t, const T&);
	Matrix(const vector<vector<T>>&);
	Matrix(const Matrix&);
	~Matrix();

	size_t size() const;
	size_t rsize() const;
	void clear();
	void fill(const T&);
	void print();
	vector<size_t> dimensions() const;

	Matrix<T> transpose() const;
	void selfIdentity();
	void selfTranspose();

	const vector<T>& operator [](size_t) const;
	vector<T>& operator [](size_t);
	
	void operator =(const vector<vector<T>>&);
	void operator =(const Matrix<T>&);
	bool operator ==(const Matrix<T>&) const;

	Matrix<T> operator +(const Matrix<T>&) const;
	Matrix<T> operator -(const Matrix<T>&) const;
	Matrix<T> operator *(const Matrix<T>&) const;
	Matrix<T> operator ^(long long);

	void operator +=(const Matrix<T>&);
	void operator -=(const Matrix<T>&);
	void operator *=(const Matrix<T>&);
	void operator ^=(long long);

	friend std::istream& operator >>(std::istream&, Matrix<T>&);
	friend std::ostream& operator <<(std::ostream&, const Matrix<T>&);

	static Matrix<T> getIdentity(size_t _row, size_t _column)
	{
		Matrix<T> _return_matrix(_row, _column, (T)0);
		for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
			_return_matrix[_row_i][_row_i] = 1;
		return _return_matrix;
	}

	static Matrix<T> getZeros(size_t _row, size_t _column)
	{
		Matrix<T> _return_matrix(_row, _column, (T)0);
		return _return_matrix;
	}
	
private:
	size_t _row_size, _column_size;
	vector<vector<T>> matrix;
};

template<typename T>
Matrix<T> ::Matrix() { _row_size = _column_size = 0; }

template<typename T>
Matrix<T> ::Matrix(size_t row, size_t column) 
{ 
	_row_size = row;
	_column_size = column;
	matrix = vector<vector<T>>(row, vector<T>(column)); 
}

template<typename T>
Matrix<T> ::Matrix(size_t row, size_t column, const T& value) 
{
	_row_size = row;
	_column_size = column;
	matrix = vector<vector<T>>(row, vector<T>(column, value)); 
}

template<typename T>
Matrix<T> ::Matrix(const vector<vector<T>>& other) 
{ 
	_row_size = other.size();
	_column_size = _row_size == 0 ? 0 : other[0].size();
	matrix = other; 
}

template<typename T>
Matrix<T> ::Matrix(const Matrix& other) 
{ 
	_row_size = other._row_size;
	_column_size = other._column_size;
	matrix = other.matrix; 
}

template<typename T>
Matrix<T> ::~Matrix() { matrix.~vector(); }

template<typename T>
size_t Matrix<T> ::size() const { return _row_size; }

template<typename T>
size_t Matrix<T> ::rsize() const { return _column_size; }

template<typename T>
void Matrix<T> ::clear() 
{
	_row_size = 0;
	_column_size = 0;
	matrix.~vector(); 
}

template<typename T>
void Matrix<T> ::fill(const T& value) 
{ 
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			matrix[_row_i][_col_i] = value;
}

template<typename T>
void Matrix<T> ::print()
{
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++) {
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			printf("%d ", matrix[_row_i][_col_i]);
		printf("\n");
	}
}

template<typename T>
vector<size_t> Matrix<T> ::dimensions() const { return { _row_size, _column_size }; }

template<typename T>
Matrix<T> Matrix<T> ::transpose() const 
{
	if(_row_size == 0 || _column_size == 0) return Matrix<T>();
	Matrix<T> _return_matrix(_column_size, _row_size);
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			_return_matrix[_col_i][_row_i] = matrix[_row_i][_col_i];
	return _return_matrix;
}

template<typename T>
void Matrix<T> ::selfIdentity()
{
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			matrix[_row_i][_col_i] = (_row_i == _col_i);
}

template<typename T>
void Matrix<T> ::selfTranspose()
{
	Matrix<T> _transposed_matrix = this->transpose();
	matrix.~vector();
	matrix = _transposed_matrix;
	std::swap(_row_size, _column_size);
}

template<typename T>
const vector<T>& Matrix<T> ::operator[](size_t index) const { return matrix[index]; }

template<typename T>
vector<T>& Matrix<T> ::operator[](size_t index) { return matrix[index]; }

template<typename T>
void Matrix<T> ::operator=(const vector<vector<T>>& other)
{
	matrix.~vector();
	_row_size = other.size();
	_column_size = _row_size == 0 ? 0 : other[0].size();
	matrix = other;
}

template<typename T>
void Matrix<T> ::operator=(const Matrix<T>& other)
{
	matrix.~vector();
	_row_size = other.size();
	_column_size = _row_size == 0 ? 0 : other[0].size();
	matrix = other.matrix;
}

template<typename T>
bool Matrix<T> ::operator==(const Matrix<T>& other) const 
{ return _row_size == other._row_size && _column_size == other._column_size && matrix == other.matrix; }

template<typename T>
Matrix<T> Matrix<T> ::operator+(const Matrix<T>& other) const
{
	if (_row_size != other._row_size || _column_size != other._column_size) return Matrix<T>();
	Matrix<T> _return_matrix(_row_size, _column_size);
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			_return_matrix[_row_i][_col_i] = matrix[_row_i][_col_i] + other.matrix[_row_i][_col_i];
	return _return_matrix;
}

template<typename T>
Matrix<T> Matrix<T> ::operator-(const Matrix<T>& other) const
{
	if (_row_size != other._row_size || _column_size != other._column_size) return Matrix<T>();
	Matrix<T> _return_matrix(_row_size, _column_size);
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			_return_matrix[_row_i][_col_i] = matrix[_row_i][_col_i] - other.matrix[_row_i][_col_i];
	return _return_matrix;
}

template<typename T>
Matrix<T> Matrix<T> ::operator*(const Matrix<T>& other) const
{
	if (_column_size != other._row_size) return Matrix<T>();
	Matrix<T> _return_matrix(_row_size, other._column_size);
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < other._column_size; _col_i++)
			for(size_t _mid_i = 0; _mid_i < _column_size; _mid_i++)
			_return_matrix[_row_i][_col_i] += matrix[_row_i][_mid_i] * other.matrix[_mid_i][_col_i];
	return _return_matrix;
}

template <typename T>
Matrix<T> Matrix<T> :: operator^(long long _power) {
	if (_row_size != _column_size) return Matrix<T>();
	Matrix<T> _result_matrix = getIdentity(_row_size, _column_size);
	Matrix<T> _helper_matrix(matrix);
	for (; _power > 0; _power >>= 1, _helper_matrix *= _helper_matrix)
		if (_power & 1) _result_matrix *= _helper_matrix;
	return _result_matrix;
}

template<typename T>
void Matrix<T> ::operator+=(const Matrix<T>& other)
{
	if (_row_size != other._row_size || _column_size != other._column_size) return;
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			matrix[_row_i][_col_i] += other.matrix[_row_i][_col_i];
}

template<typename T>
void Matrix<T> ::operator-=(const Matrix<T>& other)
{
	if (_row_size != other._row_size || _column_size != other._column_size) return;
	for (size_t _row_i = 0; _row_i < _row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _column_size; _col_i++)
			matrix[_row_i][_col_i] -= other.matrix[_row_i][_col_i];
}

template<typename T>
void Matrix<T> ::operator*=(const Matrix<T>& other)
{
	if (_column_size != other._row_size) return;
	Matrix<T> _helper_matrix = *this * other;
	matrix.~vector();
	matrix = std::move(_helper_matrix.matrix);
}

template <typename T>
void Matrix<T> ::operator^=(long long _power) {
	Matrix<T> _result_matrix = getIdentity(_row_size, _column_size);
	Matrix<T> _helper_matrix(matrix);
	for (; _power > 0; _power >>= 1, _helper_matrix *= _helper_matrix)
		if (_power & 1) _result_matrix *= _helper_matrix;
	matrix = std::move(_result_matrix.matrix);
}

template <typename T>
std::istream& operator >>(std::istream& _istream, Matrix<T>& _matrix)
{
	for (size_t _row_i = 0; _row_i < _matrix._row_size; _row_i++)
		for (size_t _col_i = 0; _col_i < _matrix._column_size; _col_i++)
			_istream >> _matrix.matrix[_row_i][_col_i];
	return _istream;
}

template <typename T>
std::ostream& operator <<(std::ostream& _ostream, const Matrix<T>& _matrix)
{
	for (size_t _row_i = 0; _row_i < _matrix._row_size; _row_i++) {
		for (size_t _col_i = 0; _col_i < _matrix._column_size; _col_i++)
			_ostream << _matrix.matrix[_row_i][_col_i] << ' ';
		_ostream << '\n';
	}
	return _ostream;
}