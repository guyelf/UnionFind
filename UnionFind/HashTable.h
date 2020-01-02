

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "serverList.h"

//key = serverId
//Data = server itself (= D)

//helper enum to resize function
enum resize_enum {DOWN,UP};



template<class D>
class HashTable
{
	serverList<D>* dynamic_arr;
	unsigned int size;
	int num_elements;
	double factor;

	void resize(resize_enum r_flag);
	int getHash(unsigned int index, int newSize = 0);
	serverNode<D>* getNode(int key);
	
public:
	explicit HashTable (unsigned int size=2);
	void insert(D data, int key);//inserts a cpy not reference 
	D& find(int key);
	bool exists(int key);//key is server id
	void remove(int key);

	//for debugging
	serverList<D>* getArray() { return this->dynamic_arr; }
	serverList<D> getItem(int index)
	{
		if (index < this->size || index >= 0)
			return this->dynamic_arr[index];
			
	}
	int getSize() { return this->size; }
	//end of debugging 
	~HashTable();
	
};

template <class D>
void HashTable<D>::resize(resize_enum r_flag)
{
	double increase_by = (r_flag == UP) ? 2 : 0.5;
	int newSize = int(increase_by*this->size);
	auto newArr = new serverList<D>[newSize];

	for (int i = 0; i < this->size; ++i) //the size of the old array
	{
		if (this->dynamic_arr[i].getSize()==0)//if this is an empty cell
			continue;

		//runs on all the servers in the cells of the dynamic list and pushes them to their new location in the new HT
		auto list_element= this->dynamic_arr[i].popHead();
		while (list_element != nullptr)
		{
			auto new_index = getHash(list_element->getId(),newSize);
			newArr[new_index].insertLast(list_element);
			list_element = this->dynamic_arr[i].popHead();
		}
	}
	delete[] this->dynamic_arr;//removes the old array
	this->size *= increase_by; //increased by the factor (2 or 0.5 the original size)
	this->dynamic_arr =  newArr;//assigns the new array
}

template <class D>
int HashTable<D>::getHash(unsigned index, int newSize)
{
	if (newSize != 0)
		return (index%newSize);

	return index % this->size;
}

template <class D>
serverNode<D>* HashTable<D>::getNode(int key)
{
	if (!exists(key))
	{
		//todo: throw Failure
	}
	int ht_index = getHash(key);

	auto cur_server = this->dynamic_arr[ht_index].getHead();

	while (cur_server->getId() != key)
		cur_server = cur_server->next;

	return cur_server;
}

template <class D>
HashTable<D>::HashTable(unsigned int size):size(size),num_elements(0),factor(0.5)
{
	dynamic_arr = new serverList<D>[this->size];
}



template <class D >
void HashTable<D>::insert(D data, int key)
{
	if(exists(key))
	{
		//throw Failure
	}
	++(this->num_elements);
	this->factor = (num_elements) / static_cast<double>(size);
	int ht_index = getHash(key);

	D* cpy_data = new D(data);
	serverNode<D>* node = new serverNode<D>(cpy_data, key);
	this->dynamic_arr[ht_index].insertLast(node);

	if (this->factor >= 1)
		resize(UP);
}

template <class D>
D& HashTable<D>::find(int key)
{
	if (!exists(key))
	{
		//todo: throw Failure
	}
	int ht_index = getHash(key);

	auto cur_server = this->dynamic_arr[ht_index].getHead();
	
	while (cur_server->getId() != key)
		cur_server = cur_server->next;
	
	return *(cur_server->getData());
}

//assumes that there's an overload function for != on HT's data w/ int
//and that there's an overload function to compare elements with ints
template <class D>
bool HashTable<D>::exists(int key)
{
	int id = getHash(key);
	auto cur_element = this->dynamic_arr[id].getHead();
	while (cur_element != nullptr && *(cur_element->getData()) != key)
		cur_element = cur_element->next;

	return cur_element? false : true;
}

template <class D>
void HashTable<D>::remove(int key)
{

	if (!exists(key))
	{
		//throw Failure
	}


	int id = getHash(key);

	serverNode<D>* node = this->getNode(key);

	this->dynamic_arr[id].removeNode(node);
	delete node; //no removal in list-node

	--(this->num_elements);

	this->factor = num_elements / static_cast<double>(this->size);
	if (this->factor <= 0.25 )
		resize(DOWN);
}


template <class D>
HashTable<D>::~HashTable()
{
	delete[] this->dynamic_arr;
}


#endif // HASHTABLE_H
