//BST.cpp

//#include <malloc.h>
#include <iostream>
#include "binary_tree.hpp"
#include "print_binary_tree.hpp"
//the node of a tree, included in binary_tree.h
#include <stdlib.h>
#include <time.h>
using namespace std;

//create node with value, left child and right child
struct bst_node* create_bst_node(int value, struct bst_node* l, struct bst_node* r){
    struct bst_node* t =(struct bst_node*) malloc(sizeof *t);//should call free() when finished
    t->value = value;
    t->l = l;
    t->r = r;
    return t;
}

struct bst_node* bst_insert(struct bst_node* root, int value){
    if(root == NULL)
        return create_bst_node(value,NULL,NULL);//tree is empty
    //if root is not NULL
    if(value < root->value)
        root->l = bst_insert(root->l,value);//insert into left child
    else if (value >= root->value)
        root->r = bst_insert(root->r,value);//
    return root;
}

int main(){
    //create_bst_node();
    srand((unsigned)time(NULL));//使用系统时间初试化随机种子
    int value = rand()%123;
    value = 5;
    struct bst_node* bst_root = create_bst_node(value, NULL, NULL);
//    for(int k=0; k<33; k++){
//        value = rand()%123;
//        bst_insert(bst_root, value);
//    }
//    bst_insert(bst_root, 5);
    bst_insert(bst_root, 3);
    bst_insert(bst_root, 7);
    bst_insert(bst_root, 2);
    bst_insert(bst_root, 6);
    bst_insert(bst_root, 8);
    
    print_binary_tree(bst_root);
    
    return 0;
}
