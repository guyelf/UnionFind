

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "AVLTree.h"

//key = serverId
//Data = server itself
template<class D>
class HashTable
{
	D* dynamic_arr;
	unsigned int size;
	unsigned int num_filled;
	//size is the old size
	AVLTree<int, D>* resize(AVLTree<int, D>* oldArr);
	int hashed_index(unsigned int index);
	
public:
	explicit HashTable (unsigned int size);
	void insert(D data, int serv_id);
	T find(int servId);
	void remove(int servId);
	~HashTable();
	
};

template <class D>
AVLTree<int,D>* HashTable<D>::resize(AVLTree<int, D>* oldArr)
{
	auto newArr = new AVLTree<int, D>*[2 * this.size];
	for (int i = 0; i < this.size; ++i) //the size of the old array
	{
		
		auto cur_server_id = oldArr[i].getNext();
		while (oldArr[i].getNext() != nullptr)
		{
			newArr[i % (2 * this.size)] = oldArr[i];
			auto cur_server_id = oldArr[i].getNext();
		}
		
	}
	delete [] oldArr;
	return newArr;
}

template <class D>
int HashTable<D>::hashed_index(unsigned index)
{
	return (index % this->size);
}

template <class D>
HashTable<D>::HashTable(unsigned int size):size(size),num_filled(0)
{
	dynamic_arr = new AVLTree<int,D>[this.size];
}



template <class D >
void HashTable<D>::insert(D data, int serv_id)
{
	int ht_index = hashed_index(serv_id);
	//checks if this is a new location utilized in the HashTable - might need to change according to the function name that will give me the size of the tree
	if(this->dynamic_arr[ht_index].getRank() == 0)
		++this->num_filled;
	

	if (this.size / 2 < this->num_filled)
	{
		auto tmp_ptr = new AVLTree<int, D>[2 * this->size];
		this->dynamic_arr = resize(tmp_ptr, this.dynamic_arr, this.size);
		delete tmp_ptr;//bug? - trying just to delete the redundant ptr
	}

	this->dynamic_arr[hashed_index];
	
}


template <class D>
HashTable<D>::~HashTable()
{
	delete[] this->dynamic_arr;
}


#endif // HASHTABLE_H
