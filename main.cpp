#include <iostream>
#include <fstream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <chrono>
#include <thread>
#include "windows.h"
#include "winnt.h"
#include "winbase.h"
#include <algorithm>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace std;
using namespace std::chrono;

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

template <class T>
class AVLNode
{
    public:
        T data;
        AVLNode *left;
        AVLNode *right;
        int height;
};

template <class T>
class AvlTree
{

    public:
        double insertion_rotation_time;
        double insertion_search_time;
        double deletion_rotation_time;
        double deletion_search_time;

    int size;
    int maxImbalance;
    int numberOfRotations;
    bool debugOutput;
    AVLNode<T> *root;

    public:

        AvlTree(int g, bool d)
        {
            maxImbalance = g;
            size = 0;
            root = NULL;
            numberOfRotations = 0;
            debugOutput = d;
        }

        int getHeight(AVLNode<T>*);
        AVLNode<T> *rightRotation(AVLNode<T>*);
        AVLNode<T> *leftRotation(AVLNode<T>*);
        AVLNode<T> *leftRightRotation(AVLNode<T>*);
        AVLNode<T> *rightLeftRotation(AVLNode<T>*);
        AVLNode<T> *insertHelper(AVLNode<T>*, T);
        void insertData(T);
        void preOrderHelper(AVLNode<T>*);
        void preOrder();
        void inOrderHelper(AVLNode<T>*);
        void inOrder();
        void postOrderHelper(AVLNode<T>*);
        void postOrder();
        int getBalance(AVLNode<T>*);
        AVLNode<T> *balanceTree(AVLNode<T>*);
        bool searchHelper(T, AVLNode<T> *);
        bool searchData(T);
        bool isEmpty();
        int getSize();
        T getMax(T, T);
        T deleteData(T);
        AVLNode<T> *deleteHelper(AVLNode<T> *, T);
        T getMinimum(AVLNode<T> *);
        void DestroyRecursive(AVLNode<T> *);
        int getNumberOfRotations();
        void clearNumRotations();
        ~AvlTree<T>();
};

//https://stackoverflow.com/questions/34170164/destructor-for-binary-search-tree
template <class T>
void AvlTree<T>::DestroyRecursive(AVLNode<T> *node)
{
    if (node)
    {
        DestroyRecursive(node->left);
        DestroyRecursive(node->right);

        if(debugOutput == true){
            cout<<"DELETING NODE WITH DATA: ";
            cout<<node->data<<endl;
        }
        delete node;
    }
}

template <class T>
int AvlTree<T>::getHeight(AVLNode<T> *node) {

   if (node == NULL)
       return -1;

    return node->height;
}

template <class T>
int AvlTree<T>::getBalance(AVLNode<T> *node) {

   int l_height = getHeight(node->left);
   int r_height = getHeight(node->right);

   return l_height - r_height;
}

template <class T>
T AvlTree<T>::getMax(T a, T b) {

    if(a > b)
        return a;

    else if(b > a)
        return b;

    return a;
}

template <class T>
AVLNode<T> *AvlTree<T>::rightRotation(AVLNode<T> *node) {

    if(debugOutput == true) {
        cout<<"RIGHT ROTATION"<<endl;
    }

    AVLNode<T> *newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;
    newRoot->height = getMax(getHeight(newRoot->left), node->height) + 1;

    numberOfRotations++;
    return newRoot;
}

template <class T>
AVLNode<T> *AvlTree<T>::leftRotation(AVLNode<T> *node) {

    if(debugOutput == true) {
        cout<<"LEFT ROTATION"<<endl;
    }

    AVLNode<T> *newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;
    newRoot->height = getMax(getHeight(newRoot->right), node->height) + 1;

    numberOfRotations++;
    return newRoot;
}

template <class T>
AVLNode<T> *AvlTree<T>::leftRightRotation(AVLNode<T> *node) {

    if(debugOutput == true) {
        cout<<"LEFTRIGHT ROTATION"<<endl;
    }

    AVLNode<T> *nodeLeftChild = node->left;
    node->left = leftRotation(nodeLeftChild);

    return rightRotation(node);
}

template <class T>
AVLNode<T> *AvlTree<T>::rightLeftRotation(AVLNode<T> *node) {

    if(debugOutput == true) {
        cout<<"RIGHTLEFT ROTATION"<<endl;
    }

    AVLNode<T> *nodeRightChild = node->right;
    node->right = rightRotation(nodeRightChild);

    return leftRotation(node);
}

template <class T>
AVLNode<T> *AvlTree<T>::balanceTree(AVLNode<T> *node) {

    int balance = getBalance(node);

    if(balance > maxImbalance) {

      if(getBalance(node->left) > 0)
            return rightRotation(node);
      else
            return leftRightRotation(node);
    }

    else if(balance < -1*maxImbalance) {

        if(getBalance(node->right) > 0)
            return rightLeftRotation(node);
        else
            return leftRotation(node);
    }

    return node;
}

template <class T>
AVLNode<T>* AvlTree<T>::insertHelper(AVLNode<T> *node, T data) {

    double start = GetCounter();
    if(node == NULL) {
        if(debugOutput == true)
            cout<<"Inserting blank node"<<endl;

        node = new AVLNode<T>;
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;

        return node;
    }

    else if(data < node->data) {
        if(debugOutput == true)
            cout<<"inserting node in left"<<endl;
        node->left = insertHelper(node->left, data);
    }

    else if(data >= node->data) {
        if(debugOutput == true)
            cout<<"inserting node in right"<<endl;
        node->right = insertHelper(node->right, data);
    }
    double end = GetCounter();

    insertion_search_time = end - start;

    node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;

    double rot_start = GetCounter();
    AVLNode<T>* newRoot = balanceTree(node);
    double rot_end = GetCounter();

    insertion_rotation_time = rot_end - rot_start;

    return newRoot;
}

template <class T>
void AvlTree<T>::insertData(T key) {

    insertion_rotation_time = 0;
    insertion_search_time = 0;
    root = insertHelper(root, key);
    size++;
}

template <class T>
void AvlTree<T>::preOrderHelper(AVLNode<T> *node) {

    if(node == NULL) {
        return;
    }

    cout << node->data << " ";
    preOrderHelper(node->left);
    preOrderHelper(node->right);
}

template <class T>
void AvlTree<T>::inOrderHelper(AVLNode<T> *node) {

    if(node == NULL) {
        return;
    }

    inOrderHelper(node->left);
    cout << node->data << " ";
    inOrderHelper(node->right);
}

template <class T>
void AvlTree<T>::postOrderHelper(AVLNode<T> *node) {

    if(node == NULL) {
        return;
    }

    postOrderHelper(node->left);
    postOrderHelper(node->right);
    cout << node->data << " ";
}

template <class T>
void AvlTree<T>::preOrder() {
    preOrderHelper(root);
}

template <class T>
void AvlTree<T>::inOrder() {
    inOrderHelper(root);
}

template <class T>
void AvlTree<T>::postOrder() {
    postOrderHelper(root);
}

template <class T>
bool AvlTree<T>::searchHelper(T key, AVLNode<T> *node) {

    if(node == NULL) {
        return false;
    }

    if(node->data == key) {
        return true;
    }

    else if(key < node->data) {
        return searchHelper(key,node->left);
    }

    return searchHelper(key, node->right);
}

template <class T>
bool AvlTree<T>::isEmpty() {
    return size == 0;
}

template <class T>
bool AvlTree<T>::searchData(T data) {

   if(isEmpty()) {
        return false;
   }

    return searchHelper(data, root);
}

template <class T>
int AvlTree<T>::getSize() {
    return size;
}

template <class T>
T AvlTree<T>::getMinimum(AVLNode<T> *node) {

   if(node->left == NULL)
       return node->data;

   while(node->left != NULL)
       node = node->left;

   return node->data;
}

template <class T>
AVLNode<T> *AvlTree<T>::deleteHelper(AVLNode<T> *node, T data) {

    if(debugOutput == true) {
        cout<<"Deletion...DELETING "<<data<<endl;
    }

    double start = GetCounter();
    if(node == NULL)
        return NULL;

    if(data < node->data) {

        if(debugOutput == true) {
            cout<<"Deletion...going left"<<endl;
        }

        node->left = deleteHelper(node->left, data);
    }

    else if(data > node->data) {

        if(debugOutput == true) {
            cout<<"Deletion...going right"<<endl;
        }

        node->right = deleteHelper(node->right, data);
    }

    else {

        if(debugOutput == true) {
            cout<<"Deletion...found it"<<endl;
        }


        if(node->left != NULL && node->right != NULL) {

            if(debugOutput == true) {
                cout<<"Deletion...Both children exist...finding inOrderSuccessor and deleting that"<<endl;
            }

            int inOrderSuccessor = getMinimum(node->right);
            node->data = inOrderSuccessor;
            node->right = deleteHelper(node->right, inOrderSuccessor);
        }

        else {

            if(node->left == NULL && node->right == NULL) {

                if(debugOutput == true) {
                    cout<<"Deletion...leaf node...set node to NULL"<<endl;
                }

                node = NULL;
            }

            else if(node->left == NULL) {

                if(debugOutput == true) {
                    cout<<"Deletion...right node exists..setting node to node.right"<<endl;
                }
                node = node->right;
            }

            else if(node->right == NULL) {

                if(debugOutput == true) {
                    cout<<"Deletion...left node exists..setting node to node.left"<<endl;
                }
                node = node->left;
            }
        }
    }
    double end = GetCounter();

    deletion_search_time = end - start;

    if(node == NULL)
        return NULL;

    node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;

    double rot_start = GetCounter();
    AVLNode<T>* newRoot = balanceTree(node);
    double rot_end = GetCounter();

    deletion_rotation_time = rot_end - rot_start;

    return newRoot;
}

template <class T>
T AvlTree<T>::deleteData(T data) {

    if(isEmpty())
        throw "Trying to delete from Empty tree";

    if(searchData(data) == false)
        return NULL;

    deletion_rotation_time = 0;
    deletion_search_time = 0;

    root = deleteHelper(root, data);
    size--;

    return data;
}

template <class T>
void AvlTree<T>::clearNumRotations() {

    if(debugOutput == true)
        cout <<"Clearing numberOfRotations"<<endl;
    numberOfRotations = 0;
}

template <class T>
int AvlTree<T>::getNumberOfRotations() {
    return numberOfRotations;
}

template <class T>
AvlTree<T>::~AvlTree<T>() {
    DestroyRecursive(root);
}

int main()
{

    /*AvlTree<float> oneTree(1, false);

    oneTree.insertData(25.12);
    cout<<endl<<"Pre-Order: ";
    oneTree.preOrder();
    cout<<endl<<"In-Order: ";
    oneTree.inOrder();
    cout<<endl<<"Post-Order: ";
    oneTree.postOrder();
    cout<<endl;

    oneTree.insertData(10.12);
    cout<<endl<<"Pre-Order: ";
    oneTree.preOrder();
    cout<<endl<<"In-Order: ";
    oneTree.inOrder();
    cout<<endl<<"Post-Order: ";
    oneTree.postOrder();
    cout<<endl;

    oneTree.insertData(8.12);
    cout<<endl<<"Pre-Order: ";
    oneTree.preOrder();
    cout<<endl<<"In-Order: ";
    oneTree.inOrder();
    cout<<endl<<"Post-Order: ";
    oneTree.postOrder();
    cout<<endl;

    oneTree.insertData(35.12);
    cout<<endl<<"Pre-Order: ";
    oneTree.preOrder();
    cout<<endl<<"In-Order: ";
    oneTree.inOrder();
    cout<<endl<<"Post-Order: ";
    oneTree.postOrder();
    cout<<endl;

    oneTree.insertData(30.12);
    cout<<endl<<"Pre-Order: ";
    oneTree.preOrder();
    cout<<endl<<"In-Order: ";
    oneTree.inOrder();
    cout<<endl<<"Post-Order: ";
    oneTree.postOrder();
    cout<<endl;

    oneTree.insertData(40.12);
    cout<<endl<<"Pre-Order: ";
    oneTree.preOrder();
    cout<<endl<<"In-Order: ";
    oneTree.inOrder();
    cout<<endl<<"Post-Order: ";
    oneTree.postOrder();
    cout<<endl;*/

    /*int numberOfElements = 10000;
    int data[numberOfElements];
    srand(0);

    for(int i = 0; i < numberOfElements; i++) {

        bool uniqueData;
        int newData;

        do {
          uniqueData = true;
          newData = rand();

          for(int j = 0; j < i; j++) {

             if(data[j] == newData) {
                uniqueData = false;
                break;
             }
          }
        } while(!uniqueData);

        data[i] = newData;
    }

     for(int g = 1; g <= 20; g++) {

        AvlTree<int> gTree(g, false);

        for(int d = 0; d < numberOfElements; d++) {
            gTree.insertData(data[d]);
        }

        random_shuffle(data, data + numberOfElements);

        for(int d = 0; d < numberOfElements; d++) {
            gTree.deleteData(data[d]);

            if(gTree.searchData(data[d]) != false){
                exit(-1);
            }
        }

     }
     */

    StartCounter();

    int numberOfElements = 1000;
    int data[numberOfElements];
    srand(0);

    for(int i = 0; i < numberOfElements; i++) {

        bool uniqueData;
        int newData;

        do {
          uniqueData = true;
          newData = rand();

          for(int j = 0; j < i; j++) {

             if(data[j] == newData) {
                uniqueData = false;
                break;
             }
          }
        } while(!uniqueData);

        data[i] = newData;
    }


    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

    char buffer [80];
    strftime (buffer,80,"%m-%d-%Y-%H-%M-%S.txt",now);

    std::ofstream dataFile;
    dataFile.open(buffer);

    if(dataFile.is_open()){

        for(int g = 1; g <= 20; g++) {

            AvlTree<float> gTree(g, false);
            double searchTime = 0;
            double rotationTime = 0;

            double start = GetCounter();
            for(int d = 0; d < numberOfElements; d++) {
                gTree.insertData(data[d]);
                searchTime += gTree.insertion_search_time;
                rotationTime += gTree.insertion_rotation_time;
            }
            double end = GetCounter();
            double elapsed = end - start;
            cout <<"TIME TAKEN: " << elapsed << endl;
            int numRotations = gTree.getNumberOfRotations();

            gTree.clearNumRotations();

            double deletion_searchTime = 0;
            double deletion_rotationTime = 0;

            double delete_start = GetCounter();
            for(int d = numberOfElements - 1; d >= 0; d--) {
                gTree.deleteData(data[d]);
                deletion_searchTime += gTree.deletion_search_time;
                deletion_rotationTime += gTree.deletion_rotation_time;
            }
            double delete_end = GetCounter();
            double delete_elapsed = end - start;
            cout <<"TIME TAKEN FOR DELETION: " << delete_elapsed << endl;
            int delete_numRotations = gTree.getNumberOfRotations();

            string dataEntry("");
            dataEntry = dataEntry + patch::to_string(g) + "," + patch::to_string(numRotations) + "," + patch::to_string(elapsed) + "," + patch::to_string(searchTime) + "," + patch::to_string(rotationTime) +
                                    "," + patch::to_string(delete_numRotations) + "," + patch::to_string(delete_elapsed) + "," + patch::to_string(deletion_searchTime) + "," + patch::to_string(deletion_rotationTime);
            cout<<dataEntry<<"\n";
            dataFile<<dataEntry<<"\n";
        }
    }

    return 0;
}
