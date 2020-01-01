

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "serverList.h"

//key = serverId
//Data = server itself (= D)

//helper enum to resize function
enum resize_enum {DOWN,UP };



template<class D>
class HashTable
{
	D* dynamic_arr;
	unsigned int size;
	unsigned int num_filled;
	//size is the old size
	serverList<D>* resize(serverList<D>* oldArr, resize_enum r_flag);
	int hashed_index(unsigned int index, int newSize = 0);
	
public:
	explicit HashTable (unsigned int size=1);
	void insert(D data, int server_id);//inserts a cpy not reference 
	D& find(int server_id);
	bool exists(int server_id);//todo: implement
	void remove(int server_id);
	~HashTable();
	
};

template <class D>
serverList<D>* HashTable<D>::resize(serverList<D>* oldArr, resize_enum r_flag)
{
	double factor = (r_flag == UP) ? 2 : 0.5;


	auto newArr = new serverList<D>*[factor * this.size];
	for (int i = 0; i < this.size; ++i) //the size of the old array
	{
		if (oldArr[i] == nullptr)//if this is an empty cell
			continue;

		//runs on all the servers in the cells of the dynamic list and pushes them to their new location in the new HT
		auto cur_server= oldArr[i].popHead();
		while (cur_server != nullptr)
		{
			auto new_index = hashed_index(cur_server->getServerId(),factor*this->size);
			newArr[new_index]->insertFirst(cur_server);
			cur_server = oldArr[i].popHead();
		}
	}
	delete [] oldArr;
	this->size *= factor; //increased by the factor (2 or 0.5 the original size)
	return newArr;
}

template <class D>
int HashTable<D>::hashed_index(unsigned index, int newSize)
{
	if (newSize != 0)
		return (index%newSize);

	return index % this->size;
}
template <class D>
HashTable<D>::HashTable(unsigned int size):size(size),num_filled(0)
{
	dynamic_arr = new serverList<D>*[this.size];
}



template <class D >
void HashTable<D>::insert(D data, int server_id)
{
	int ht_index = hashed_index(server_id);
	//checks if this is a new location utilized in the HashTable - might need to change according to the function name that will give me the size of the tree
	if(this->dynamic_arr[ht_index]->getHead() == nullptr)
		++this->num_filled;
	

	if (this.size - 1 <= this->num_filled)
		this->dynamic_arr = resize(this.dynamic_arr, UP);
		

	this->dynamic_arr[hashed_index]->insertLast(data);//data should be the server
}

template <class D>
D& HashTable<D>::find(int server_id)
{
	int ht_index = hashed_index(server_id);
	//checks if this is a new location utilized in the HashTable - might need to change according to the function name that will give me the size of the tree
	auto cur_server = this.dynamic_arr[ht_index]->getHead();
	if (cur_server == nullptr)
		//todo throw Failure

	//assumes that the tail->next == nullptr
	while (cur_server != nullptr && cur_server->getServerId() != server_id)
	{
		cur_server = cur_server->getNext();
	}
	if (cur_server == nullptr)
		//todo throw Failure

	return cur_server;
}

template <class D>
void HashTable<D>::remove(int server_id)
{
	auto server = this->find(server_id);

	this->dynamic_arr[ht_index].removeNode(server);
	this->num_filled--;
	if (this.size * 0.25 > this->num_filled)
		this->dynamic_arr = resize(this.dynamic_arr, DOWN);
}


template <class D>
HashTable<D>::~HashTable()
{
	delete[] this->dynamic_arr;
}


#endif // HASHTABLE_H
