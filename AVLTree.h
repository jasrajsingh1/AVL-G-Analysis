#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

typedef struct _avlNode
{
    int data;
    struct _avlNode *left;
    struct _avlNode *right;
} avlNode;

class AvlTree
{
    avlNode* root;

    int size;

    public:
        int height(avlNode*);
        avlNode *rightRotation(avlNode*);
        avlNode *leftRotation(avlNode*);
        avlNode *leftRightRotation(avlNode*);
        avlNode *rightLeftRotation(avlNode*);
        avlNode *insertData(avlNode*, int);
        void preOrder(avlNode*);
        void inOrder(avlNode*);
        void postOrder(avlNode*);
        int getBalance(avlNode*);
        avlNode *balanceTree(avlNode*);
        bool search(int, avlNode *);
        bool isEmpty();
        int getSize();
        AvlTree() {
            root = NULL;
            size = 0;
        };
        ~AvlTree();
};

#endif
