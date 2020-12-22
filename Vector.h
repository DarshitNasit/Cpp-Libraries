#pragma once
// @Author : Darshit Nasit

#include<iostream>

template<typename Vector>
class Vector_Iterator
{
public:
	using _value_type = typename Vector::_value_type;
	using _pointer_type = _value_type*;
	using _reference_type = _value_type&;

public:
	Vector_Iterator(_pointer_type _ptr) : _data_ptr(_ptr) {}

	Vector_Iterator& operator++();
	Vector_Iterator operator++(int);
	Vector_Iterator& operator--();
	Vector_Iterator operator--(int);
	Vector_Iterator operator+(size_t);
	Vector_Iterator operator-(size_t);
	Vector_Iterator& operator+=(size_t);
	Vector_Iterator& operator-=(size_t);

	_reference_type operator[](size_t);
	_pointer_type operator->();
	_reference_type operator*();

	bool operator==(const Vector_Iterator&);
	bool operator!=(const Vector_Iterator&);
	bool operator<(const Vector_Iterator&);
	bool operator>(const Vector_Iterator&);
	bool operator<=(const Vector_Iterator&);
	bool operator>=(const Vector_Iterator&);

private:
	_pointer_type _data_ptr;
};

template<typename T>
class Vector
{
public:
	using _value_type = T;
	using iterator = Vector_Iterator<Vector<T>>;

public:
	Vector() { }
	Vector(size_t);
	Vector(size_t, const T&);
	Vector(size_t, T&&);
	Vector(const Vector<T>&);
	Vector(std::initializer_list<T>&& init);

	~Vector() 
	{ 
		for (size_t i = 0; i < vector_size; i++)
			vector_data[i].~T();
		delete[] vector_data;
	}

	size_t size() const;
	size_t capacity() const;
	void clear();

	const iterator front() const;
	const iterator back() const;
	iterator begin() const;
	iterator end() const;
	
	void push_back(const T&);
	void push_back(T&&);

	template<typename... Args>
	void emplace_back(Args&&...);

	void pop_back();

	const T& operator[](size_t) const;
	T& operator[](size_t);

	void operator=(const Vector<T>&);
	void operator=(Vector<T>&&);
	void operator=(std::initializer_list<T>&&);

private:
	void reAllocate(size_t);
	size_t increaseCapacity() const;
	size_t decreaseCapacity() const;

private:
	T* vector_data = nullptr;
	size_t vector_size = 0;
	size_t vector_capacity = 0;
	const static size_t INITIAL_CAPACITY = 1;
};

template<typename T>
Vector<T> ::Vector(size_t v_size)
{
	vector_size = v_size;
	vector_capacity = v_size;
	vector_data = new T[v_size];
}

template<typename T>
Vector<T> ::Vector(size_t v_size, const T& value)
{
	vector_size = v_size;
	vector_capacity = v_size;
	vector_data = new T[v_size];
	for (size_t i = 0; i < v_size; i++)
		vector_data[i] = value;
}

template<typename T>
Vector<T> ::Vector(size_t v_size, T&& value)
{
	vector_size = v_size;
	vector_capacity = v_size;
	vector_data = new T[v_size];
	for (size_t i = 0; i < v_size; i++)
		vector_data[i] = value;
}

template<typename T>
Vector<T> ::Vector(const Vector<T>& other)
{
	vector_size = other.vector_size;
	vector_capacity = other.vector_capacity;
	vector_data = new T[vector_capacity];
	for (size_t i = 0; i < vector_size; i++)
		vector_data[i] = other[i];
}

template<typename T>
Vector<T> ::Vector(std::initializer_list<T>&& init) 
{
	size_t v_size = init.end() - init.begin();
	if (v_size != 0) {
		vector_size = v_size;
		vector_capacity = v_size;
		vector_data = new T[v_size];
		for (const T* it = init.begin(); it != init.end(); it++)
			vector_data[it-init.begin()] = std::move(*it);
	}
}

template<typename T>
void Vector<T> ::reAllocate(size_t capacity) 
{
	if (capacity < vector_size)
		vector_size = capacity;

	T* new_vector_data = nullptr;
	if (capacity != 0) {
		new_vector_data = new T[capacity];
		for (size_t i = 0; i < vector_size; i++)
			new_vector_data[i] = std::move(vector_data[i]);
	}

	delete[] vector_data;
	vector_data = new_vector_data;
	vector_capacity = capacity;
}

template<typename T>
size_t Vector<T> ::increaseCapacity() const { return vector_capacity * 2; }

template<typename T>
size_t Vector<T> ::decreaseCapacity() const { return vector_capacity / 2; }

template<typename T>
size_t Vector<T> ::size() const { return vector_size; }

template<typename T>
size_t Vector<T> ::capacity() const { return vector_capacity; }

template<typename T>
void Vector<T> ::clear()
{
	for (size_t i = 0; i < vector_size; i++)
		vector_data[i].~T();

	delete[] vector_data;
	vector_data = nullptr;

	vector_size = 0;
	vector_capacity = 0;
}

template<typename T>
const typename Vector<T> ::iterator Vector<T> ::front() const
{ 
	if (vector_size == 0)
		return nullptr;
	return vector_data;
}

template<typename T>
const typename Vector<T> ::iterator Vector<T> ::back() const
{
	if (vector_size == 0)
		return nullptr;
	return &(vector_data[vector_size - 1]);
}

template<typename T>
typename Vector<T> ::iterator Vector<T> ::begin() const
{
	if (vector_size == 0)
		return nullptr;
	return Vector_Iterator<Vector<T>>(vector_data);
}

template<typename T>
typename Vector<T> ::iterator Vector<T> ::end() const
{
	if (vector_size == 0)
		return nullptr;
	return Vector_Iterator<Vector<T>>(vector_data + vector_size);
}

template<typename T>
void Vector<T> ::push_back(const T& value)
{
	if (vector_data == nullptr)
		reAllocate(INITIAL_CAPACITY);
	if (vector_size >= vector_capacity)
		reAllocate(increaseCapacity());
	vector_data[vector_size++] = value;
}

template<typename T>
void Vector<T> ::push_back(T&& value)
{
	if (vector_data == nullptr)
		reAllocate(INITIAL_CAPACITY);
	if (vector_size >= vector_capacity)
		reAllocate(increaseCapacity());
	vector_data[vector_size++] = std::move(value);
}

template<typename T>
template<typename... Args>
void Vector<T> ::emplace_back(Args&&... args)
{
	if (vector_data == nullptr)
		reAllocate(INITIAL_CAPACITY);
	if (vector_size >= vector_capacity)
		reAllocate(increaseCapacity());
	vector_data[vector_size++] = T(std::forward<Args>(args)...);
}

template<typename T>
void Vector<T> ::pop_back()
{
	if (vector_size > 0) {
		vector_data[--vector_size].~T();
		size_t decreasedCapacity = decreaseCapacity();
		if (vector_size <= decreasedCapacity)
			reAllocate(decreasedCapacity);
	}
}

template<typename T>
const T& Vector<T> ::operator[] (size_t index) const { return vector_data[index]; }

template<typename T>
T& Vector<T> ::operator[] (size_t index) { return vector_data[index]; }

template<typename T>
void Vector<T> ::operator=(const Vector<T>& other)
{
	~Vector();
	vector_size = other.vector_size;
	vector_capacity = other.vector_capacity;
	vector_data = new T[vector_size];
	for (size_t i = 0; i < vector_size; i++)
		vector_data[i] = other.vector_data[i];
}

template<typename T>
void Vector<T> ::operator=(Vector<T>&& other)
{
	~Vector();
	vector_size = other.vector_size;
	vector_capacity = other.vector_capacity;
	vector_data = new T[vector_size];
	for (size_t i = 0; i < vector_size; i++)
		vector_data[i] = std::move(other.vector_data[i]);
}

template<typename T>
void Vector<T> ::operator=(std::initializer_list<T>&& list)
{
	~Vector();
	vector_size = (size_t)(list.end() - list.begin());
	vector_capacity = vector_size;
	vector_data = new T[vector_size];
	for (const T* it = list.begin(); it != list.end(); it++)
		vector_data[it - list.begin()] = std::move(*it);
}

template<typename T>
Vector_Iterator<T>& Vector_Iterator<T> ::operator++()
{
	_data_ptr++;
	return *this;
}

template<typename T>
Vector_Iterator<T> Vector_Iterator<T> ::operator++(int)
{
	Vector_Iterator<T> _copied_iterator = *this;
	++(*this);
	return _copied_iterator;
}

template<typename T>
Vector_Iterator<T>& Vector_Iterator<T> ::operator--()
{
	_data_ptr--;
	return *this;
}

template<typename T>
Vector_Iterator<T> Vector_Iterator<T> ::operator--(int)
{
	Vector_Iterator<T> _copied_iterator = *this;
	--(*this);
	return _copied_iterator;
}

template<typename T>
Vector_Iterator<T> Vector_Iterator<T> ::operator+(size_t _off)
{
	Vector_Iterator<T> _copied_iterator = *this;
	return _copied_iterator += _off;
}

template<typename T>
Vector_Iterator<T> Vector_Iterator<T> ::operator-(size_t _off)
{
	Vector_Iterator<T> _copied_iterator = *this;
	return _copied_iterator -= _off;
}

template<typename T>
Vector_Iterator<T>& Vector_Iterator<T> ::operator+=(size_t _off)
{
	_data_ptr += _off;
	return *this;
}

template<typename T>
Vector_Iterator<T>& Vector_Iterator<T> ::operator-=(size_t _off)
{
	_data_ptr -= _off;
	return *this;
}

template<typename T>
typename Vector_Iterator<T> ::_reference_type Vector_Iterator<T> ::operator[](size_t index)
{
	return *(_data_ptr + index);
}

template<typename T>
typename Vector_Iterator<T> ::_pointer_type Vector_Iterator<T> ::operator->()
{
	return _data_ptr;
}

template<typename T>
typename Vector_Iterator<T> ::_reference_type Vector_Iterator<T> ::operator*()
{
	return *_data_ptr;
}

template<typename T>
bool Vector_Iterator<T> ::operator==(const Vector_Iterator& _other)
{
	return _data_ptr == _other._data_ptr;
}

template<typename T>
bool Vector_Iterator<T> ::operator!=(const Vector_Iterator& _other)
{
	return _data_ptr != _other._data_ptr;
}

template<typename T>
bool Vector_Iterator<T> ::operator<(const Vector_Iterator& _other)
{
	return _data_ptr < _other._data_ptr;
}

template<typename T>
bool Vector_Iterator<T> ::operator>(const Vector_Iterator& _other)
{
	return _data_ptr > _other._data_ptr;
}

template<typename T>
bool Vector_Iterator<T> ::operator<=(const Vector_Iterator& _other)
{
	return !(_data_ptr > _other._data_ptr);
}

template<typename T>
bool Vector_Iterator<T> ::operator>=(const Vector_Iterator& _other)
{
	return !(_data_ptr < _other._data_ptr);
}