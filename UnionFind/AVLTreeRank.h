//
// Created by ניב שפק on 12/1/2019.
//

#ifndef AVLTREERANK_H
#define AVLTREERANK_H

#include <cstdio>
#include "AVLTreeRankNode.h"
#include "wet1Exception.h"
#include <tgmath.h>
#include <assert.h>
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
    D *begin() {
        iterator = root;
        if (!iterator) return nullptr;
        while (iterator->getLeftSon()) {
            iterator = iterator->getLeftSon();
        }
        return iterator->getData();
    }; // set iterator to the smallest node.

    int max(int a, int b){
        if (a>b) return a;
        return b;
    }


    void updateHeight(TreeNode<K,D>* node){
        if(node== nullptr) return;
        if (node->getLeftSon()== nullptr && node->getRightSon()== nullptr) {
            node->height=0;
            node->sub_tree_size=1;
            node->sub_tree_param = node->getParam();
            return;
        }
        int r=0, l=0;
        int sub_size_left=0, sub_size_right=0;
        int sum_param_right=0, sum_param_left=0;
        if(node->getRightSon()) {
            r = node->getRightSon()->getHeight();
            sub_size_right = node->getRightSon()->sub_tree_size;
            sum_param_right = node->getRightSon()->sub_tree_param;
        }
        if(node->getLeftSon()) {
            l=node->getLeftSon()->getHeight();
            sub_size_left = node->getLeftSon()->sub_tree_size;
            sum_param_left = node->getLeftSon()->sub_tree_param;
        }
        node->height=max(l,r)+1;
        node->sub_tree_size = sub_size_left + sub_size_right + 1;
        node->sub_tree_param = sum_param_left + sum_param_right + node->getParam();
    }
    D *getNext() {
        if(this->getNextNode())
            return this->iterator->getData();
        return nullptr;
    };
    TreeNode<K, D> *getNextNode() {
        if (iterator->getRightSon()) {
            iterator = iterator->getRightSon();
            while (iterator->getLeftSon()) {
                iterator = iterator->getLeftSon();
            }
            return iterator;
        }
        TreeNode<K, D>* father_node = iterator->getFather();
        if (!father_node) return nullptr;   //***
        while (father_node->getRightSon()==iterator){
            iterator = father_node;
            father_node = father_node->getFather();
            if(!father_node) return nullptr;
        }
        iterator = father_node;
        return iterator;
    };
    TreeNode<K, D> * getRoot(){
        return this->root;
    }
    void setRoot(TreeNode<K, D> * new_root){
        this->root = new_root;
    }
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
    void addNode(K key, D* data, int param=0) {

        if (addNode_aux(key, data, root, nullptr, param)) {
            TreeNode<K, D>* new_node = new TreeNode<K, D>(key, data, nullptr, param);
            root = new_node;
        }
    }
    bool addNode_aux(K key, D* data, TreeNode<K, D>* node, TreeNode<K, D>* father, int param) {
        if (!node) return true;
        if (node->getKey() == key)
            throw FailException();
        if (node->getKey() < key) {
            if (addNode_aux(key, data, node->getRightSon(), node,param)) {
                TreeNode<K, D>* new_node = new TreeNode<K, D>(key, data, node, param);
                node->setRightSon(new_node);
            }
        }
        if (key < node->getKey()) {
            if (addNode_aux(key, data, node->getLeftSon(), node,param)) {
                TreeNode<K, D>* new_node = new TreeNode<K, D>(key, data, node, param);
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
                if(fatherNode==root) break; //thh
                balanceNode(fatherNode);
                fatherNode = fatherNode->getFather();
            }
            balanceNode(root);
            while (root->getFather())
                root = root->getFather();
            return;
        }
        while (padre!= nullptr){
            if(padre==root) break;
            updateHeight(padre);
            balanceNode(padre);
            padre = padre->getFather();
        }
        updateHeight(root);
        balanceNode(root);
        while (root->getFather())
            root = root->getFather();
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
        int tmp_param = node2->getParam();
        unsigned int tmp_sub = node1->getSubSize();
        unsigned int tmp_height = node1->height;
        node1->setData(node2->getData());
        node1->setKey(node2->getKey());
        node1->setBalance(node2->getBalance());
        node1->height = node2->height;
        node1->sub_tree_size = node2->sub_tree_size;
        node2->setParam(node1->getParam());
        node1->setParam(tmp_param);
        node2->setData(tmp_data);
        node2->setKey(tmp_key);
        node2->setBalance(tmp_balance);
        node2->height = tmp_height;
        node2->sub_tree_size = tmp_sub;
    }
    void clear(TreeNode<K, D>* sub_tree_root);
    void deleteTreeData();
    void deleteTreeData_aux(TreeNode<K, D>* node);
    TreeNode<K, D>* findKNode(int k){
        if (k<=0) return nullptr;
        if (!root) return nullptr;
        if (root->sub_tree_size<k) return nullptr;
        TreeNode<K, D>* it = root;
        while (true){
            if (it->getLeftSon()== nullptr && it->getRightSon()== nullptr && k==1) return it;
            if (it->getLeftSon() && it->getLeftSon()->sub_tree_size==k-1) return it;
            if (it->getLeftSon() && it->getLeftSon()->sub_tree_size>k-1) it = it->getLeftSon();
            else {
                k-=1;
                if(it->getLeftSon()) {
                    k -= it->getLeftSon()->sub_tree_size;
                }
                it = it->getRightSon();
            }
        }
    }
    int getSumParamBigK(int k){
        if (!root) return 0;
        int K_sum = root->sub_tree_param;
        if (root->sub_tree_size<=k) return K_sum;
        K k_node_key = findKNode(root->sub_tree_size-k+1)->getKey();
        TreeNode<K, D>* it = root;
        while (it->getKey()!= k_node_key){
            if (it->getKey() < k_node_key){
                if (it->getLeftSon())
                    K_sum -= it->getLeftSon()->sub_tree_param;
                K_sum -= it->getParam();
                it = it->getRightSon();
            }
            else it = it->getLeftSon();
        }
        if (it->getLeftSon())
            K_sum -= it->getLeftSon()->sub_tree_param;
        return K_sum;
    }
    void mergeTrees(AVLTree* tree2);
    TreeNode<K,D>** arrayInOrder();
    TreeNode<K,D>* buildEmptyTree(int size);
    void updateTreeOn(TreeNode<K,D>* node);
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
    node_left_son->setFather(fatherNode);
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
        delete (node->getData());
    }

}

template <class K, class D>
void AVLTree<K,D>::updateTreeOn(TreeNode<K,D>* node) {
    if(!node) return;
    updateTreeOn(node->getLeftSon());
    updateTreeOn(node->getRightSon());
    updateHeight(node);
    node->reBalance();
}





template <class K, class D>
AVLTree<K, D>::~AVLTree()
{
    clear(this->root);
}

template <class K, class D>
TreeNode<K,D>** AVLTree<K, D>::arrayInOrder()
{
    if (this->root == nullptr) return nullptr;
    int size = this->root->sub_tree_size;
    TreeNode<K,D>** array = new TreeNode<K,D>*[size];
    iterator = this->min(this->root);
    array[0] = iterator;
    for (int i=1 ; i<size ; i++){
        array[i] = this->getNextNode();
    }
    return array;
}

template<class K, class D>
static TreeNode<K,D>** arrayMerge(TreeNode<K,D>** array1,int size1, TreeNode<K,D>** array2, int size2){
    int size = size1+size2;
    TreeNode<K,D>** array = new TreeNode<K,D>*[size];
    int it1 = 0;
    int it2 = 0;
    for (int i=0 ; it1<size1 && it2<size2; i++){
        if (array1[it1]->getKey()<array2[it2]->getKey()){
            array[i] = array1[it1++];
            //array1[it1++]= nullptr;
        } else {
            array[i] = array2[it2++];
        }
    }
    int i = it1+it2;
    if (it1<size1){
        while (i<size){
            array[i]=array1[it1++];
            i++;
        }
    }
    if (it2<size2){
        while (i<size){
            array[i]=array2[it2++];
            i++;
        }
    }
    return array;
}

template <class K, class D>
TreeNode<K, D>* buildEmptyCompleteTree(int height) {
    if (height<0) return nullptr;
    TreeNode<K,D>* empty_tree = new TreeNode<K,D>;
    empty_tree->setFather(nullptr);
    empty_tree->setLeftSon(buildEmptyCompleteTree<K,D>(height-1));
    empty_tree->setRightSon(buildEmptyCompleteTree<K,D>(height-1));
    empty_tree->height = height;
    empty_tree->sub_tree_size = pow(2,height+1)-1;
    return empty_tree;
}

/*int minInt(int a, int b){
    if (a<b) return a;
    return b;
}*/



template <class K, class D>
bool deleteLeaves(TreeNode<K,D>* tree, int remove){ //
    if (remove == 0) return false;
    if ((tree->getHeight()==0) && (remove==1)) {
        delete(tree);
        return true;
    }
    int remove_right = remove<pow(2, tree->getRightSon()->getHeight()) ? remove:pow(2, tree->getRightSon()->getHeight());
    if (deleteLeaves(tree->getRightSon(), remove_right)){
        tree->setRightSon(nullptr);
    }
    if (deleteLeaves(tree->getLeftSon(), remove-remove_right)){
        tree->setLeftSon(nullptr);
    }
    if (remove == 2*remove_right){
        tree->height--;
    }
    return false;
}


template <class K, class D>
TreeNode<K, D>* AVLTree<K, D>::buildEmptyTree(int size) {
 if (size==0) return nullptr;
 int i=1;
 while (pow(2,i)<=size)
     i++;
 TreeNode<K, D>* empty_tree = buildEmptyCompleteTree<K,D>(i-1);
 int remove = pow(2,i)-1-size;
 deleteLeaves(empty_tree, remove);
    return empty_tree;
}


template <class K, class D>
void AVLTree<K, D>::mergeTrees(AVLTree *tree2) {
    if (tree2->getRoot() == nullptr) return;
    if (this->root == nullptr) {
        this->root=tree2->getRoot();
        tree2->setRoot(nullptr);
    }
    TreeNode<K,D>** array1 = this->arrayInOrder();
    int arr1_size = root->sub_tree_size;
    TreeNode<K,D>** array2 = tree2->arrayInOrder();
    int arr2_size = tree2->getRoot()->sub_tree_size;
    tree2->setRoot(nullptr);
    TreeNode<K,D>** array = arrayMerge(array1, arr1_size, array2, arr2_size);
    TreeNode<K,D>* new_tree = buildEmptyTree(arr1_size+arr2_size);
    //delete this->root;
    this->root = new_tree;
    this->iterator = min(new_tree);
    int i;
    for (i=0 ; i<arr1_size+arr2_size-1 ; i++){
        this->iterator->assingedNode(array[i]);
        delete array[i];
        this->getNextNode();
    }
    this->iterator->assingedNode(array[i]);
    delete array[i];
    updateTreeOn(this->root);
    this->root->setFather(nullptr);
    delete array1;
    delete array2;
    delete array;

}








#endif //AVLTREERANK_H


