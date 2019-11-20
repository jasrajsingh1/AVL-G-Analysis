#include <iostream>
#include <cstdlib>
#include "AVLTree.h"

using namespace std;

int AvlTree::height(avlNode *node) {

   if (node == NULL)
       return 0;

   int l_height = height(node->left);
   int r_height = height(node->right);
   int max_height = max(l_height, r_height);

   return max_height + 1;
}

int AvlTree::getBalance(avlNode *node) {

   int l_height = height(node->left);
   int r_height = height(node->right);

   return l_height - r_height;
}

avlNode *AvlTree::rightRotation(avlNode *node) {

    cout<<"RIGHT ROTATION"<<endl;
    avlNode *newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    return newRoot;
}

avlNode *AvlTree::leftRotation(avlNode *node) {

    cout<<"LEFT ROTATION"<<endl;
    avlNode *newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    return newRoot;
}

avlNode *AvlTree::leftRightRotation(avlNode *node) {

    cout<<"LEFTRIGHT ROTATION"<<endl;

    avlNode *nodeLeftChild = node->left;
    node->left = leftRotation(nodeLeftChild);

    return rightRotation(node);
}

avlNode *AvlTree::rightLeftRotation(avlNode *node) {

    cout<<"RIGHTLEFT ROTATION"<<endl;

    avlNode *nodeRightChild = node->right;
    node->right = rightRotation(nodeRightChild);

    return leftRotation(node);
}

avlNode *AvlTree::balanceTree(avlNode *node) {

    int balance = getBalance(node);

    if(balance > 1) {

      if(getBalance(node->left) > 0)
            return rightRotation(node);
      else
            return leftRightRotation(node);
    }

    else if(balance < -1) {

        if(getBalance(node->right) > 0)
            return rightLeftRotation(node);
        else
            return leftRotation(node);
    }

    return node;
}

avlNode* AvlTree::insertData(avlNode *node, int data) {

    if(node == NULL) {
        cout<<"Inserting blank node"<<endl;
        node = new avlNode;
        node->data = data;
        node->left = NULL;
        node->right = NULL;

        size++;
        return node;
    }

    else if(data < node->data) {
            cout<<"inserting node in left"<<endl;
        node->left = insertData(node->left, data);
        node = balanceTree(node);
    }

    else if(data >= node->data) {
            cout<<"inserting node in right"<<endl;
        node->right = insertData(node->right, data);
        node = balanceTree(node);
    }

    return node;
}

void AvlTree::preOrder(avlNode *node) {

    if(node == NULL) {
        return;
    }

    cout << node->data << " ";
    preOrder(node->left);
    preOrder(node->right);
}

void AvlTree::inOrder(avlNode *node) {

    if(node == NULL) {
        return;
    }

    inOrder(node->left);
    cout << node->data << " ";
    inOrder(node->right);
}

void AvlTree::postOrder(avlNode *node) {

    if(node == NULL) {
        return;
    }

    postOrder(node->left);
    postOrder(node->right);
    cout << node->data << " ";
}

bool AvlTree::search(int key, avlNode *node) {

    if(node == NULL) {
        return false;
    }

    if(node->data == key) {
        return true;
    }

    else if(key < node->data) {
        return search(key,node->left);
    }

    return search(key, node->right);
}

bool AvlTree::isEmpty() {
    return size == 0;
}

int AvlTree::getSize() {
    return size;
}

AvlTree::~AvlTree() {
    delete root;
}
