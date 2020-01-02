

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "unionFind_Node.h"
#include "HashTable.h"

struct HTData
{
public:
	int group_id; //the dc root id
	int param; //traffic server
	HTData(int group_id, int param = 0) : group_id(group_id), param(param) {};
};



template<class K, class D>
class UnionFInd
{
	int* parent_elements;
	int* group_size;
	unionFind_Node<K,D>** roots;
	HashTable<HTData> ht_data;
	int size;

public:
	explicit UnionFInd(unsigned int n);
	void unionGroups(unionFind_Node<K,D>* node1, unionFind_Node<K, D>* node2);
	int getSize(unionFind_Node<K, D>* node);
	int UnionFInd<K, D>::getGroupId(unionFind_Node<K, D>* node);
	void UnionFInd<K, D>::addData(int ufNode_id, int data_id);
	void removeData(int data_id);
	void setParam(int data_id, int param);
	int sumHeighestParam(int uf_node_id, int k);
	~UnionFInd();
};

template <class K, class D>
UnionFInd<K, D>::UnionFInd(unsigned n):size(n)
{
	parent_elements = new int[n+1];
	for (int i = 0; i < n+1; ++i)
		parent_elements[i] = i;

	group_size = new int[n + 1];
	for (int i = 0; i < n + 1; ++i)
		group_size[i] = 1;

	roots = new unionFind_Node<K,D>*[n + 1];
	for (int i = 0; i < n + 1; ++i)
		roots[i] = new unionFind_Node<K,D>(i);
}

template <class K, class D>
 void UnionFInd<K, D>::unionGroups(unionFind_Node<K, D>* node1, unionFind_Node<K, D>* node2)
{
	if (this->getSize(node1) < this->getSize(node2))
		unionGroups(node2, node1);

	node1->union_nodes(node2);
	//adds the size from the root of node2 to the size of the root of node1
	this->group_size[this->getGroupId(node1)] += this->group_size[this->getGroupId(node2)];
	// sets root of node2 to be root of node1
	this->parent_elements[this->getGroupId(node2)] = this->parent_elements[this->getGroupId(node1)]; 
}

template <class K, class D>
int UnionFInd<K, D>::getGroupId(unionFind_Node<K, D>* node)
{
	int id = node->getId();
	while (this->parent_elements[id] != id)
		id = this->parent_elements[id];
	return id;
}

template <class K, class D>
void UnionFInd<K, D>::addData(int ufNode_id, int data_id)
{
	HTData ht_element (ufNode_id);
	this->ht_data.insert(ht_element, data_id);
}

template <class K, class D>
void UnionFInd<K, D>::removeData(int data_id)
{
	auto ht_element	 = ht_data.find(data_id);//the server to remove
	if (ht_element.param != 0) {
		DataKey to_remove(data_id, ht_element.param);
		int group = this->getGroupId(ht_element.group_id);
		this->roots[group]->get_tree()->removeNodeByKey(to_remove);
		this->roots[0]->get_tree()->removeNodeByKey(to_remove);
	}
	this->ht_data.remove(data_id);
}

template <class K, class D>
void UnionFInd<K, D>::setParam(int data_id, int param)
{	
	int uf_node = ht_data.find(data_id).group_id;
	int group = this->getGroupId(uf_node);
	this->removeData(data_id);
	HTData new_ht(group, param);
	this->ht_data.insert(new_ht, data_id);
	if (param != 0) {
		DataKey new_key(data_id, param);
		this->roots[group]->get_tree()->addNode(new_key, nullptr, param);
		this->roots[0]->get_tree()->addNode(new_key, nullptr, param);
	}
}

template <class K, class D>
int UnionFInd<K, D>::sumHeighestParam(int uf_node_id, int k)
{
	int group = this->getGroupId(uf_node_id);
	return this->roots[group]->get_tree()->getSumParamBigK(k);
}

template <class K, class D>
int UnionFInd<K, D>::getSize(unionFind_Node<K, D>* node)
{	
	int id = this->getGroupId(node);
	return this->group_size[id];
}

template <class K, class D>
UnionFInd<K, D>::~UnionFInd()
{
	delete[] this->group_size;
	delete[] this->parent_elements;
	for (int i = 0; i < this->size + 1; ++i)//n+1
		delete[] this->roots[i];

}

#endif // UNIONFIND_H



