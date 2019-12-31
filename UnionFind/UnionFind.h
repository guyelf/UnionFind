

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include "unionFind_Node.h"


template<class K, class D>
class UnionFInd
{

	int* parent_elements;
	int* group_size;
	unionFind_Node<D>** roots;
public:
	UnionFInd(unsigned int n);



};

template <class K, class D>
UnionFInd<K, D>::UnionFInd(unsigned n)
{
	parent_elements = new int[n+1];
	for (int i = 0; i < n+1; ++i)
		parent_elements[i] = i;

	group_size = new int[n + 1];
	for (int i = 0; i < n + 1; ++i)
		group_size[i] = 1;

	roots = new UnionFInd*[n + 1];
	for (int i = 0; i < n + 1; ++i)
		roots[i] = new unionFind_Node<D>();

}

#endif // UNIONFIND_H



