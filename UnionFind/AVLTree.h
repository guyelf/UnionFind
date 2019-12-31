//
// Created by ניב שפק on 12/1/2019.
//

#ifndef AVLTREE_H
#define AVLTREE_H

#include <cstdio>
#include "TreeNode.h"

enum { L_balance = 2, R_balance = -2 };

template <class K, class  D>
class AVLTree {
	TreeNode<K, D> *root;
	TreeNode<K, D> *iterator;

	TreeNode<K, D> *rollLR(TreeNode<K, D> *node);

	TreeNode<K, D> *rollRL(TreeNode<K, D> *node);

	TreeNode<K, D> *rollRR(TreeNode<K, D> *node);

	TreeNode<K, D> *rollLL(TreeNode<K, D> *node);

public:
	AVLTree<K, D>() : root(nullptr) {};
	// add d'tor and maybe operators

	D *begin() {
		iterator = root;
		if (!iterator) return nullptr;
		while (iterator->getLeftSon()) {
			iterator = iterator->getLeftSon();
		}
		return iterator->getData();
	}; // set iterator to the smallest node.
    void updateHeight(TreeNode<K,D>* node){
        if(node== nullptr) return;
        if (node->getLeftSon()== nullptr && node->getRightSon()== nullptr) {
            node->height=0;
            return;
        }
        int r=0, l=0;
        if(node->getRightSon()) r=node->getRightSon()->getHeight();
        if(node->getLeftSon()) l=node->getLeftSon()->getHeight();
        node->height=(l-r);
    }


	D *getNext() {
        if (iterator->getRightSon()) {
            iterator = iterator->getRightSon();
            while (iterator->getLeftSon()) {
                iterator = iterator->getLeftSon();
            }
            return iterator->getData();
        }
		TreeNode<K, D>* father_node = iterator->getFather();
        if (!father_node) return nullptr;   //***
		while (father_node->getRightSon()==iterator){
		    iterator = father_node;
		    father_node = father_node->getFather();
		    if(!father_node) return nullptr;
		}
		iterator = father_node;
        return iterator->getData();
	};
	TreeNode<K, D> *min(TreeNode<K, D> *node) {
		if (!node) return nullptr;
		TreeNode<K, D> *min = node;
		while (min->getLeftSon()) {
			min = min->getLeftSon();
		}
		return min;
	}; // return ptr to the min node
	TreeNode<K, D>* max(TreeNode<K, D> *node) {
		if (!node) return nullptr;
		TreeNode<K, D>* max = node;
		while (max->getRightSon()) {
			max = max->getRightSon();
		}
		return max;
	}; // return ptr to the max node
	bool balanceNode(TreeNode<K, D>* node) {
		if (!node) return false;
		node->reBalance();
		if (node->getBalance()>2 || node->getBalance()<-2)
		    printf("\n\n\n\n\n\n\naaaaaaaaaaaaaaaaaaaaaaaa\n\n\n\n\n\n");
		if (node->getBalance() == 2) {
			if (node->getLeftSon()->getBalance() == -1) {
				rollLR(node);
			}
			else {
				rollLL(node);
			}
		}
		else if (node->getBalance() == -2) {
			if (node->getRightSon()->getBalance() == 1) {
				rollRL(node);
			}
			else {
				rollRR(node);
			}

		}
		else return false;
		return true;
	};
	TreeNode<K, D>* findNode(K key) {
		TreeNode<K, D>* tmp = root;
		while (tmp && tmp->getKey() != key) {
			if (tmp->getKey() < key) tmp = tmp->getRightSon();
			else tmp = tmp->getLeftSon();
		}
		return tmp;
	}
	D* getDataByKey(K key){
        TreeNode<K, D>* node = this->findNode(key);
        if(!node) return nullptr;
        return node->getData();
	};
	void addNode(K key, D* data) {

		if (addNode_aux(key, data, root, nullptr)) {
			TreeNode<K, D>* new_node = new TreeNode<K, D>(key, data, nullptr);
			root = new_node;
		}
	}
	bool addNode_aux(K key, D* data, TreeNode<K, D>* node, TreeNode<K, D>* father) {
		if (!node) return true;
		if (node->getKey() == key)
		    throw FailException();
		if (node->getKey() < key) {
			if (addNode_aux(key, data, node->getRightSon(), node)) {
				TreeNode<K, D>* new_node = new TreeNode<K, D>(key, data, node);
				node->setRightSon(new_node);
			}
		}
		if (key < node->getKey()) {
			if (addNode_aux(key, data, node->getLeftSon(), node)) {
				TreeNode<K, D>* new_node = new TreeNode<K, D>(key, data, node);
				node->setLeftSon(new_node);
			}
		}
		if(node->getLeftSon()) updateHeight(node->getLeftSon());
		if(node->getRightSon()) updateHeight(node->getRightSon());
		updateHeight(node);
		if (balanceNode(node) && node == root) {
			root = node->getFather();//todo: bug?
			root->setFather(nullptr);
		}
		return false;
	}
	void removeNodeByKey(K key) {
        TreeNode<K, D>* node = findNode(key);
        if (node) removeNodeByPtr(node);
		else
		    throw FailException();
	}
    void removeNodeByPtr(TreeNode<K, D>* node) {
        TreeNode<K, D>* padre = nullptr;
	    if(!node->getRightSon()&&!node->getLeftSon()){
            if (node == root) {
                root = nullptr;
            }
            else if (node->getFather()->getRightSon() == node) {
                padre = node->getFather();
                node->getFather()->setRightSon(nullptr);
            }
            else {
                padre = node->getFather();
                node->getFather()->setLeftSon(nullptr);
            }
            delete(node);
        } else if (!node->getLeftSon() || !node->getRightSon()) {
            if (node == root) {
                if (node->getLeftSon()) {
                    root = node->getLeftSon();
                    node->getLeftSon()->setFather(nullptr);
                } else {
                    root = node->getRightSon();
                    node->getRightSon()->setFather(nullptr);
                }
            } else if (node->getFather()->getRightSon() == node) {
                padre = node->getFather();
                if (node->getRightSon()) {
                    node->getFather()->setRightSon(node->getRightSon());
                    node->getRightSon()->setFather(node->getFather());
                } else {
                    node->getFather()->setRightSon(node->getLeftSon());
                    node->getLeftSon()->setFather(node->getFather());
                }
            } else {
                padre = node->getFather();
                if (node->getRightSon()) {
                    node->getFather()->setLeftSon(node->getRightSon());
                    node->getRightSon()->setFather(node->getFather());
                } else {
                    node->getFather()->setLeftSon(node->getLeftSon());
                    node->getLeftSon()->setFather(node->getFather());
                }
            }
        delete(node);
	    }
        else {              //has two sons, find the next and swap them
            padre = node->getFather();
            TreeNode<K, D> *next = node->getRightSon();
            while (next->getLeftSon()) {
                next = next->getLeftSon();
            }
            swapNode(node, next);
            if(next->getRightSon()){
                if(next->getFather()->getLeftSon()==next) {
                    next->getFather()->setLeftSon(next->getRightSon());
                    next->getRightSon()->setFather(next->getFather());
                } else{
                    next->getFather()->setRightSon(next->getRightSon());
                    next->getRightSon()->setFather(next->getFather());
                }
            } else {
                if(next->getFather()->getLeftSon()==next){
                    next->getFather()->setLeftSon(nullptr);
                } else {
                    next->getFather()->setRightSon(nullptr);
                }
            }
            TreeNode<K, D>* fatherNode = next->getFather();

            updateHeight(fatherNode);
            padre = fatherNode;
            while(padre->getFather()){
                padre = padre->getFather();
                updateHeight(padre);
            }
            delete(next);
            while (fatherNode!= nullptr){
                if(fatherNode!=root) break;
                balanceNode(fatherNode);
                fatherNode = fatherNode->getFather();
            }
            balanceNode(root);
            return;
        }
        updateHeight(padre);
        while (padre!= nullptr){
            if(padre!=root) break;
            updateHeight(padre);
            balanceNode(padre);
            padre = padre->getFather();
        }
        balanceNode(root);

    }
    TreeNode<K, D>* removeNode_aux(K key, TreeNode<K, D>* node){
	    if (!node) return nullptr;
	    if (node->getKey()==key){
                return node;
	    }
	    if (node->getKey()<key){
	        return removeNode_aux(key, node->getRightSon());
	    }
        return removeNode_aux(key, node->getLeftSon());
	}
	void updatePathBalance(TreeNode<K, D> *node) {
		while (!node) {
			node->reBalance();
			node = node->getFather();
		}
	};
	void swapNode(TreeNode<K, D> *node1, TreeNode<K, D> *node2) {
		K tmp_key = node1->getKey();
		D* tmp_data = node1->getData();
        int tmp_balance = node1->getBalance();
        unsigned int tmp_sub = node1->getSubSize();
        unsigned int tmp_height = node1->height;
		node1->setData(node2->getData());
		node1->setKey(node2->getKey());
		node1->setBalance(node2->getBalance());
		node1->height = node2->height;
		node1->sub_tree_size = node2->sub_tree_size;

		node2->setData(tmp_data);
		node2->setKey(tmp_key);
        node2->setBalance(tmp_balance);
		node2->height = tmp_height;
		node2->sub_tree_size = tmp_sub;
	}

	void clear(TreeNode<K, D>* sub_tree_root);
	void deleteTreeData();
	void deleteTreeData_aux(TreeNode<K, D>* node);
	~AVLTree();
};

//gets a ptr the unbalanced node in the tree (sub-tree root)
//returns a ptr of sub-tree root after LL rotation was performed
//function assumes that the tree is unbalanced
template <class K, class D>
TreeNode<K, D>* AVLTree<K, D>::rollLL(TreeNode<K, D>* node)
{
	auto fatherNode = node->getFather();//saving ptr for later
	auto node_left_son = node->getLeftSon();

	node->setLeftSon(node_left_son->getRightSon()); //setting as new son

	node_left_son->setRightSon(node);


	//fixing the father
	if (fatherNode != nullptr)// not Tree root
	{
		node_left_son->setFather(fatherNode);
	
		//if the initial node was the left son of the father, else ->right son of the father
		if (fatherNode->getLeftSon() == node)
			fatherNode->setLeftSon(node_left_son);
		else
			fatherNode->setRightSon(node_left_son);

	}
    updateHeight(node);
    updateHeight(node_left_son);
    return node_left_son;
}


//gets a ptr the unbalanced node in the tree (sub-tree root)
//returns a ptr of sub-tree root after RR rotation was performed.
//function assumes that the tree is unbalanced
template <class K, class D>
TreeNode<K, D>* AVLTree<K, D>::rollRR(TreeNode<K, D>* node)
{
	auto node_right_son = node->getRightSon();
	auto fatherNode = node->getFather(); //saving ptr for later


	node->setRightSon(node_right_son->getLeftSon());
	//   node_right_son->getLeftSon()->setFather(node);

	node_right_son->setLeftSon(node);

	if (fatherNode != nullptr)// not tree root
	{
		node_right_son->setFather(fatherNode);
		if (fatherNode->getLeftSon() == node)
			fatherNode->setLeftSon(node_right_son);
		else
			fatherNode->setRightSon(node_right_son);
	}

	//setting the father of the node last to avoid messing up the setting of the father for the 'node_right_son'
	node->setFather(node_right_son);
	updateHeight(node);
	updateHeight(node_right_son);
	return node_right_son;
}

//gets a ptr the unbalanced node in the tree (sub-tree root)
//returns a ptr of sub-tree root after LR rotation was performed.
//function assumes that the tree is unbalanced
template <class K, class D>
TreeNode<K, D>* AVLTree<K, D>::rollLR(TreeNode<K, D>* node)
{
	auto fatherNode = node;
	auto left_son = node->getLeftSon();
	left_son->setBalance(R_balance);//setting R_balance to perform rr rotation
	/*auto root_sub_tree_after_rr = */rollRR(left_son);
	//current root still has L_balance applied
	return rollLL(fatherNode);
}

template <class K, class D>
TreeNode<K, D>* AVLTree<K, D>::rollRL(TreeNode<K, D>* node)
{
	auto fatherNode = node;
	auto right_son = node->getRightSon();
	right_son->setBalance(L_balance);
	/*auto after_LL =*/ rollLL(right_son);
	auto after_RL = rollRR(fatherNode);
	return after_RL;
}

template <class K, class D>
void AVLTree<K, D>::clear(TreeNode<K, D>* sub_tree_root)
{
	if (sub_tree_root == nullptr)
		return;

	//first removing the whole left part
	clear(sub_tree_root->getLeftSon());

	//then the right part
	clear(sub_tree_root->getRightSon());

	//delete the leaf
	delete sub_tree_root;
}

template <class K, class D>
void AVLTree<K, D>::deleteTreeData()
{
	deleteTreeData_aux(this->root);
}

template <class K, class D>
void AVLTree<K, D>::deleteTreeData_aux(TreeNode<K, D>* node)
{
	if (node)
	{
		deleteTreeData_aux(node->getLeftSon());
		deleteTreeData_aux(node->getRightSon());
		delete node->getData();
	}

}

template <class K, class D>
AVLTree<K, D>::~AVLTree()
{
	clear(this->root);
}


#endif //AVLTREE_H


