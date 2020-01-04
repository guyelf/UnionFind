

#ifndef UNIONFIND_NODE_H
#define UNIONFIND_NODE_H


#include "AVLTreeRank.h"


struct DataKey
{
	int id;
	int traffic;
	
	DataKey(int id=-1, int traffic =0):id(id), traffic(traffic){};

	bool operator<(const DataKey& sk) const
	{
		if(this->traffic == sk.traffic)
			return this->id < sk.id;

		return this->traffic < sk.traffic;
	};
	bool operator<=(const DataKey& sk) const
	{
		return (*this < sk || *this == sk);
	};
	bool operator==(const DataKey& sk) const
	{
		return (this->id == sk.id && this->traffic == sk.traffic);
	};
	bool operator!=(const DataKey& sk) const
	{
		return !(*this == sk);
	};
};




template<class D>
class unionFind_Node //a data center
{
	int id;
	AVLTree<DataKey,D> dataTree; //will hold the hash table
	unionFind_Node* father;
public:
	explicit unionFind_Node(int id=-1) :id(id), father(nullptr){};
	void union_nodes(unionFind_Node* node);
	void set_father(unionFind_Node* node);
	AVLTree<DataKey, D>* get_tree();
	int getId()
	{
		return this->id;
	}
};


template <class D>
void unionFind_Node<D>::union_nodes(unionFind_Node* node)
{
	node->set_father(this);
	this->dataTree.mergeTrees(node->get_tree());
}

template <class D>
void unionFind_Node<D>::set_father(unionFind_Node* node)
{
	this->father = node;
}

template <class D>
AVLTree<DataKey,D>* unionFind_Node<D>::get_tree()
{
	return &this->dataTree;
}


#endif // UNIONFIND_NODE_H

