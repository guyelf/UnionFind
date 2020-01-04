//
// Created by ניב שפק on 12/1/2019.
//

#ifndef MIV_WET1_SERVERLIST_H
#define MIV_WET1_SERVERLIST_H


//This part is for the Node of the List
template <class T>
struct serverNode {
private:
	T* data;
	int id;
public:
	serverNode* next;
	serverNode* prev;
    serverNode(T* data, int id):data(data), id(id), next(nullptr), prev(nullptr){};
	//explicit serverNode(const T* data) :data(data), next(nullptr), prev(nullptr) {};
	T* getData();
	int getId() const;
    ~serverNode(){
        delete data;
    }
};


template <class T>
 T* serverNode<T>::getData()
{
	return this->data;
}

template <class T>
int serverNode<T>::getId() const
{
	return this->id;
}


////This Part is for the List

template <class T>
class serverList {
	serverNode<T>* head;
	serverNode<T>* tail;
	int size;
public:
	serverList() : head(nullptr), tail(nullptr), size(0) {};

	int getSize() const;
	 serverNode<T>* getTail();
	 serverNode<T>* getHead();

	void insertLast(serverNode<T>* newNode);
	void insertFirst(serverNode<T>* newNode);
	//when allocating a new server
    serverNode<T>* popHead();
	//gets a ptr of a node in the list, removes it
	void removeNode(serverNode<T>* serverNode);
	~serverList(){
	    serverNode<T>* tmp = this->head;
	    while(tmp!= nullptr){
	        delete tmp;
	        this->popHead();
	        tmp = this->head;
	    }
	}
};

template <class T>
int serverList<T>::getSize() const
{
	return this->size;
}

template <class T>
serverNode<T>* serverList<T>::getTail()
{
	return this->tail;
}

template <class T>
serverNode<T>* serverList<T>::getHead()
{
	return this->head;
}

template <class T>
void serverList<T>::insertLast(serverNode<T>* newNode)
{
	newNode->next = nullptr;
	newNode->prev = nullptr;

	if(this->size == 0)
	{
		this->head = newNode;
		this->tail = newNode;
		++(this->size);
	}
	else
	{
		this->tail->next = newNode;
		newNode->prev = this->tail;
		this->tail = newNode;
		++(this->size);
	}
}

template <class T> //might need to change everything to . (dots) instead of arrows (->) bc it's all ptrs now
void serverList<T>::insertFirst(serverNode<T>* newNode)
{
	//init
	newNode->prev = nullptr;

	if (this->size == 0)
	{
		this->head = newNode;
		this->tail = this->head;
		++this->size;
	}
	else
	{
		newNode->next = this->head;
		this->head = newNode;

		++this->size;
	}

}

template <class T>
serverNode<T>* serverList<T>::popHead()
{
	if (size == 0 ) return nullptr;
    if (size == 1){
        auto tempHead = this->head;
        this->head = nullptr;
        this->tail = nullptr;
        tempHead->next = nullptr;
        this->size--;
        return tempHead;
    }
	auto tempHead = this->head;
	this->head = this->head->next;
	this->head->prev = nullptr;

	tempHead->next = nullptr;
	tempHead->prev = nullptr;

	--this->size;
    return tempHead;
}

template <class T>
void serverList<T>::removeNode(serverNode<T>* serverNode)
{
    if(this->head == serverNode) {
        this->popHead();
        return;
    }
    if(serverNode->next){
        serverNode->next->prev = serverNode->prev;
    }
	//TODO: NOTE! this change here!
	//before: serverNode->prev = serverNode->next;
    serverNode->prev->next = serverNode->next;
    serverNode->prev = nullptr;
    serverNode->next = nullptr;
    --(this->size);
}




#endif //MIV_WET1_SERVERLIST_H
