



template <class D>
class unionFind_Node //a data center
{
	int id;
	D data; //will hold the hash table 
	int group_id;
	int group_size;
	unionFind_Node* father;

};