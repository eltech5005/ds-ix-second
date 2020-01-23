/* 
** Binary Search Tree implementation in C++
** Harish R
*/
#include<iostream>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <thread>
#include <mutex>

using namespace std;

const int BST_ELEMENTS_NUMBER = rand() % 100;
const int BST_MAX_ELEMENT_VALUE = 1000;
const int BST_THREADS_NUMBER = rand() % 30;
const int BST_THREADS_OPERATIONS = 100;

mutex lock_tree;

class BST {
    
    struct node {
        int data;
        node* left;
        node* right;
    };

    node* root;

    node* makeEmpty(node* t) {
        if(t == NULL)
            return NULL;
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        return NULL;
    }

    node* insert(int x, node* t)
    {
        if(t == NULL)
        {
            t = new node;
            t->data = x;
            t->left = t->right = NULL;
        }
        else if(x < t->data)
            t->left = insert(x, t->left);
        else if(x > t->data)
            t->right = insert(x, t->right);
        return t;
    }

    node* findMin(node* t)
    {
        if(t == NULL)
            return NULL;
        else if(t->left == NULL)
            return t;
        else
            return findMin(t->left);
    }

    node* findMax(node* t) {
        if(t == NULL)
            return NULL;
        else if(t->right == NULL)
            return t;
        else
            return findMax(t->right);
    }

    node* remove(int x, node* t) {
        node* temp;
        if(t == NULL)
            return NULL;
        else if(x < t->data)
            t->left = remove(x, t->left);
        else if(x > t->data)
            t->right = remove(x, t->right);
        else if(t->left && t->right)
        {
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->data, t->right);
        }
        else
        {
            temp = t;
            if(t->left == NULL)
                t = t->right;
            else if(t->right == NULL)
                t = t->left;
            delete temp;
        }

        return t;
    }

    void inorder(node* t) {
        if(t == NULL)
            return;
        inorder(t->left);
        cout << t->data << " ";
        inorder(t->right);
    }

    node* find(node* t, int x) {
        if(t == NULL)
            return NULL;
        else if(x < t->data)
            return find(t->left, x);
        else if(x > t->data)
            return find(t->right, x);
        else
            return t;
    }

public:
    BST() {
        root = NULL;
    }

    ~BST() {
        root = makeEmpty(root);
    }

    void insert(int x) {
        lock_tree.lock();
        root = insert(x, root);
        lock_tree.unlock();
    }

    void remove(int x) {
        lock_tree.lock();
        root = remove(x, root);
        lock_tree.unlock();
    }

    void display() {
        lock_tree.lock();
        inorder(root);
        lock_tree.unlock();
        cout << endl;
    }

    void display(node* node) {
        inorder(node);
        cout << endl;
    }

    node* search(int x) {
        node* tmp;
        lock_tree.lock();
        tmp = find(root, x);
        lock_tree.unlock();
        return tmp;
    }
};

void t_thread (BST* t) {
    for (int i = 0; i < BST_THREADS_OPERATIONS; i++){
        int action = rand() % 3;
        switch (action)
        {
        case 0:
            t->insert(rand() % BST_MAX_ELEMENT_VALUE);
            break;
        
        case 1:
            t->search(rand() % BST_MAX_ELEMENT_VALUE);
            break;
        
        case 2:
            t->remove(rand() % BST_MAX_ELEMENT_VALUE);
            break;

        default:
           break;
        }
    }
}

int main() {

    BST t;

    for (int i = 0; i < BST_ELEMENTS_NUMBER; i++)
      t.insert(rand() % BST_MAX_ELEMENT_VALUE);

    t.display();

    srand(time(NULL));    

    thread th[BST_THREADS_NUMBER];

    for (int thread_c = 0; thread_c < BST_THREADS_NUMBER; thread_c++){
        th[thread_c] = thread(t_thread, &t);
    }

    for (int thread_c = 0; thread_c < BST_THREADS_NUMBER; thread_c++)
      th[thread_c].join();

    t.display();

    return 0; 
}