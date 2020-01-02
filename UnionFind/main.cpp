#include <iostream>
#include "AVLTreeRank.h"
#include "HashTable.h"


int main() {


	auto HT = new HashTable<int>();

	int a = 1;

	for (int i = 1; i < 7; ++i)
	{
		
		HT->insert(i, i);		
		a *= 2;
	}

	//HT->remove(32);
	//HT->remove(256);
	//HT->remove(1);
	//HT->remove(4);
	//
	//HT->remove(4096);
	//HT->remove(8192);
	HT->remove(1);
	HT->remove(2);
	HT->remove(3);
	HT->remove(4);
	//HT->remove(5);
	HT->remove(6);


	for (int i = 0; i < HT->getSize(); ++i)
	{
		std::cout << "Cur i= " << i << " "<<std::endl;
		auto list_element = HT->getItem(i).getHead();
		if(list_element != nullptr)
		{
			int count = 0;
			while(list_element)
			{
				++count;
				std::cout << *(list_element->getData()) << " ";
				std::cout << list_element->getId() <<" element # " << count <<std::endl;
				list_element = list_element->next;
			}

		}

	}
    return 0;
}