

#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
#include <stdexcept>
template<class T>
class DynamicArray
{
	T* ptr;
	unsigned int size;
	unsigned int num_filled;
public:
	DynamicArray<T>(unsigned int size);

	void insert(T data, unsigned int index);
	T find(unsigned int index);
	void remove(unsigned int index);
	//T& operator[](unsigned int index); can't control the size of the array this way
	~DynamicArray();
};

template <class T>
DynamicArray<T>::DynamicArray<T>(unsigned size):size(size),num_filled(0)
{
	ptr = new T[size];
}

template <class T>
void DynamicArray<T>::insert(T data, unsigned index)
{
	if (size <= index)
		throw std::out_of_range("Index is too big");
	this->num_filled++;
	if (this->size/2 < this->num_filled)// need to increase the array
	{
		
	}
	ptr[index] = data;
	

}

template <class T>
void DynamicArray<T>::remove(unsigned index)
{
}



/*
template <class T>
T& DynamicArray<T>::operator[](unsigned int index)
{
	if (size <= index)
		throw std::out_of_range("Index too big");
	return ptr[index];
}
*/
template <class T>
DynamicArray<T>::~DynamicArray()
{
}

#endif // DYNAMICARRAY_H
