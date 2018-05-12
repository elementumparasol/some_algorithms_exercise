//
//  M_RBTree.hpp
//  testBinarySearchTree
//
//  Created by tombp on 10/05/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#ifndef M_RBTree_hpp
#define M_RBTree_hpp

#include <stdio.h>

typedef struct M_RBTree{
    int key;
    bool bRed;
    M_RBTree* left;
    M_RBTree* right;
    M_RBTree* p;
}M_RBTree;

typedef struct M_RBTree_P{
    int level;
    int key;
    int pre_space;
    bool bRed;
    M_RBTree_P *left;
    M_RBTree_P *right;
    M_RBTree_P *p;
}M_RBTree_P;

class M_C_RB_Tree {
//protected:
public:
    M_RBTree *p_root;
    M_RBTree_P *p_root_p;
public:
    M_C_RB_Tree();
    ~M_C_RB_Tree();
    void Insert(int key);
    void CopyTree2TreeP();
    void FixSpacesCountTreeP();
    void Print();
    
protected:
    M_RBTree *RB_mini_stack[300];
    int RB_mini_top;
    M_RBTree_P *RBP_mini_stack[800];     //为什么改成 800 会出现越界
    int RBP_mini_top;
    int mini_top;
    int mini_head;
    int mini_tail;
};

#endif /* M_RBTree_hpp */
