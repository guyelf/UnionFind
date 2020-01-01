
#ifndef AVL_TREE_RANK_NODE_H
#define AVL_TREE_RANK_NODE_H

#include "wet1Exception.h"

template <class K, class  D>
class TreeNode {
    K key;
    D* data;
    TreeNode* father;
    TreeNode* left_son;
    TreeNode* right_son;
    int balance;
    int param;
public:
    unsigned int sub_tree_param;
    unsigned int sub_tree_size;
    unsigned int height;
    TreeNode<K, D>(){};
    TreeNode<K, D>(K key, D* data, TreeNode<K, D> *father, int param=0) : key(key), data(data), father(father),
                                                             left_son(nullptr), right_son(nullptr), balance(0),
                                                             param(param), sub_tree_param(param),
                                                             sub_tree_size(1), height(0) {}
    ~TreeNode() = default;
    // add maybe operators
    unsigned int getSubSize() const;

    //this values just return the data they hold, can't think of a way that a node as a root will calculate its decedents height properly.
    //can only be done when inserting a new node to the TREE (AVLTREE) and then scanning the tree upwards and re-assigning the values --> so I don't think there should be a method here that calculates this.
    unsigned int getHeight() const;
    unsigned int getLeftSubHeight() const;
    unsigned int getRightSubHeight() const;
    int getBalance() const;
    TreeNode* getLeftSon();
    TreeNode* getRightSon();
    TreeNode* getFather();
    void setKey(K key) {
        this->key = key;
    };
    void setData(D *data) {
        this->data = data;
    };
    void setLeftSon(TreeNode* node);
    void setRightSon(TreeNode* node);
    void assingedNode(TreeNode<K,D>* node){
        this->setData(node->getData());
        this->setKey(node->getKey());
        this->param = node->getParam();
    }
    void setFather(TreeNode* node); // sets the father node of the son
    //updates the balance of the nodes in the tree //todo: change name to updateBalance once the code is compiled.
    void reBalance() {
        int left = 0, right = 0;
        if (this->getLeftSon()) {
            left = (this->getLeftSon()->getHeight());
        }
        if (this->getRightSon()) {
            right = (this->getRightSon()->getHeight());
        }
        if (left > right) {
            this->height = (left + 1);
        }
        else {
            this->height = (right + 1);
        }
        this->balance = (int)left - (int)right;
    };

    //regular setter
    void setBalance(int b) {
        this->balance = b;
    }
    K getKey() {return this->key; }
    int getParam(){ return this->param;}
    D* getData() { return this->data; }
    void swapWithFather() {
        TreeNode<K, D>* prevFather = this->getFather();
        if (!prevFather) return;
        TreeNode<K, D>* gFather = prevFather->getFather();
        if (!gFather) {
            if (gFather->getLeftSon() == prevFather) {
                gFather->setLeftSon(this);
            }
            else {
                gFather->setRightSon(this);
            }
            this->setFather(gFather);
            gFather->setFather(this);
            if (prevFather->getLeftSon() == this) {
                prevFather->setLeftSon(this->getLeftSon());
                this->setLeftSon(prevFather);
                TreeNode<K, D>* tmp = prevFather->getRightSon();
                prevFather->setRightSon(this->getRightSon());
                this->setLeftSon(tmp);
                tmp->setFather(this);
                prevFather->getRightSon()->setFather(prevFather);
                prevFather->getLeftSon()->setFather(prevFather);
            }
            else {
                prevFather->setRightSon(this->getRightSon());
                this->setRightSon(prevFather);
                TreeNode<K, D>* tmp = prevFather->getLeftSon();
                prevFather->setLeftSon(this->getLeftSon());
                this->setRightSon(tmp);
                tmp->setFather(this);
                prevFather->getRightSon()->setFather(prevFather);
                prevFather->getLeftSon()->setFather(prevFather);
            }
        }
    };
    //overload operator < and if key even check data.
    bool operator<(const TreeNode<K, D>* node) const
    {
        if (this->key == node->getKey())
            return this->data < node->getKey();

        return this->key < node->getKey();
    }

    //comparison between nodes
    bool operator==(const TreeNode* node)
    {
        return (node->getKey() == this->key && node->getData() == this->data);
    };

    bool operator!=(const TreeNode* node) const
    {
        return  !operator==(this, node);
    }
};

//template <class K, class D>
/*TreeNode<K, D>::~TreeNode()
{
	//delete this->key;
	delete this->data;
}*/

template <class K, class D>
unsigned int TreeNode<K, D>::getHeight() const
{
    return this->height;
}

template <class K, class D>
unsigned int TreeNode<K, D>::getSubSize() const
{
    return this->sub_tree_size;
}


template <class K, class D>
int TreeNode<K, D>::getBalance() const
{
    return this->balance;
}

template <class K, class D>
TreeNode<K, D>* TreeNode<K, D>::getLeftSon()
{
    return this->left_son;
}

template <class K, class D>
TreeNode<K, D>* TreeNode<K, D>::getRightSon()
{
    return  this->right_son;
}

template <class K, class D>
TreeNode<K, D>* TreeNode<K, D>::getFather()
{
    return this->father;
}

template <class K, class D>
void TreeNode<K, D>::setLeftSon(TreeNode* node)
{
    this->left_son = node;
    if (node) node->setFather(this);
}

template <class K, class D>
void TreeNode<K, D>::setRightSon(TreeNode* node)
{
    this->right_son = node;
    if (node) node->setFather(this);
}

template <class K, class D>
void TreeNode<K, D>::setFather(TreeNode* node)
{
    this->father = node;
}

#endif //AVL_TREE_RANK_NODE_H
