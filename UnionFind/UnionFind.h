

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "unionFind_Node.h"
#include "HashTable.h"

struct HTData
{
public:
    int group_id;
    int id;
	int param;
	HTData(int group_id,int id, int param = 0) : group_id(group_id), id(id), param(param) {};
	bool operator==(int id2){
        return this->id == id2;
	}
	bool operator!=(int id2){
        return !(*this==id2);
	}
};



template<class D>
class UnionFInd
{
	int* parent_elements;
	int* group_size;
	unionFind_Node<D>** roots;
	HashTable<HTData> ht_data;
	int size;

public:
	explicit UnionFInd(unsigned int n);
	void unionGroups(unionFind_Node<D>* node1, unionFind_Node<D>* node2);
	int getSize(unionFind_Node<D>* node);
	int getGroupId(unionFind_Node<D>* node);
	void addData(int ufNode_id, int data_id);
	void removeData(int data_id);
	void setParam(int data_id, int param);
	int sumHeighestParam(int uf_node_id, int k);
	unionFind_Node<D>* getGroup(int id);
	~UnionFInd();
	unionFind_Node<HTData>* getdc(int n){
        return this->roots[n];
	}
};

template <class D>
unionFind_Node<D>* UnionFInd<D>::getGroup(int id){
    return this->roots[id];
}



template<class D>
UnionFInd<D>::UnionFInd(unsigned n):size(n)
{
	parent_elements = new int[n+1];
	for (int i = 0; i < n+1; ++i)
		parent_elements[i] = i;

	group_size = new int[n + 1];
	for (int i = 0; i < n + 1; ++i)
		group_size[i] = 1;

	roots = new unionFind_Node<D>*[n + 1];
	for (int i = 0; i < n + 1; ++i)
		roots[i] = new unionFind_Node<D>(i);
}

template<class D>
 void UnionFInd<D>::unionGroups(unionFind_Node<D>* node1, unionFind_Node<D>* node2)
{
	if (this->getSize(node1) < this->getSize(node2))
		unionGroups(node2, node1);

	node1->union_nodes(node2);
	//adds the size from the root of node2 to the size of the root of node1
	this->group_size[this->getGroupId(node1)] += this->group_size[this->getGroupId(node2)];
	// sets root of node2 to be root of node1
	this->parent_elements[this->getGroupId(node2)] = this->getGroupId(node1);
}

template<class D>
int UnionFInd<D>::getGroupId(unionFind_Node<D>* node)
{
	int id = node->getId();
	while (this->parent_elements[id] != id)
		id = this->parent_elements[id];
	return id;
}

template<class D>
void UnionFInd<D>::addData(int ufNode_id, int data_id)
{
	HTData ht_element (ufNode_id, data_id);
	this->ht_data.insert(ht_element, data_id);
}

template<class D>
void UnionFInd<D>::removeData(int data_id)
{
	auto ht_element	 = ht_data.find(data_id);//the server to remove
	if (ht_element.param != 0) {
		DataKey to_remove(data_id, ht_element.param);
		int group = this->getGroupId(this->getGroup(ht_element.group_id));
		this->roots[group]->get_tree()->removeNodeByKey(to_remove);
		this->roots[0]->get_tree()->removeNodeByKey(to_remove);
	}
	this->ht_data.remove(data_id);
}

template<class D>
void UnionFInd<D>::setParam(int data_id, int param)
{	
	int group = this->getGroupId(this->getGroup(ht_data.find(data_id).group_id));
	this->removeData(data_id);
	HTData new_ht(group,data_id, param);
	this->ht_data.insert(new_ht, data_id);
	if (param != 0) {
		DataKey new_key(data_id, param);
		this->roots[group]->get_tree()->addNode(new_key, nullptr, param);
		this->roots[0]->get_tree()->addNode(new_key, nullptr, param);
	}
}

template<class D>
int UnionFInd<D>::sumHeighestParam(int uf_node_id, int k)
{
	int group = this->getGroupId(this->getGroup(uf_node_id));
	return this->roots[group]->get_tree()->getSumParamBigK(k);
}

template<class D>
int UnionFInd<D>::getSize(unionFind_Node<D>* node)
{	
	int id = this->getGroupId(node);
	return this->group_size[id];
}

template<class D>
UnionFInd<D>::~UnionFInd()
{
	delete[] this->group_size;
	delete[] this->parent_elements;
	for (int i = 0; i < this->size + 1; ++i)//n+1
		delete this->roots[i];
	delete roots;
}
#endif // UNIONFIND_H



