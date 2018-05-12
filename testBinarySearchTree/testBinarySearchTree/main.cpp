//
//  main.cpp
//  testBinarySearchTree
//
//  Created by tombp on 02/05/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#include <iostream>

#include <deque>
#include <iterator>

#include "M_RBTree.hpp"




typedef struct M_BST{
    int key;
    M_BST* left;
    M_BST* right;
    M_BST* p;
}M_BST;

void M_BST_Insert(M_BST **m_tree,int key)
{
    M_BST *m_tmp = (M_BST*)malloc(sizeof(M_BST));
    m_tmp->p = NULL;
    m_tmp->left = NULL;
    m_tmp->right = NULL;
    m_tmp->key = key;
    M_BST *m_n,*p;
    m_n = *m_tree;
    p = *m_tree;
    while (p != NULL) {
        if ( key < p->key ) {
            m_n = p;
            p = p->left;
        }else{
            m_n = p;
            p = p->right;
        }
    }
    if (m_n != NULL) {
        if (key < m_n->key ) {
            m_n->left = m_tmp;
            m_tmp->p = m_n;
        }else{
            m_n->right = m_tmp;
            m_tmp->p = m_n;
        }
    }else{
        *m_tree = m_tmp;
    }
}
void M_BST_Print(M_BST* m_tree)
{
    printf("( ");
    if (m_tree != NULL) {
        printf("[%d]",m_tree->key);
        printf("{ ");
        M_BST_Print(m_tree->left);
        printf("} ");
        printf("{ ");
        M_BST_Print(m_tree->right);
        printf("} ");
    }else{
        printf("[NIL]");
    }
    printf(") ");
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
//    M_BST *m_root = NULL;
//    M_BST_Insert(&m_root, 5);
//    M_BST_Insert(&m_root, 3);
//    M_BST_Insert(&m_root, 7);
//    M_BST_Insert(&m_root, 2);
//    M_BST_Print(m_root);
    M_C_RB_Tree m_root;
    m_root.Insert(300);
    m_root.Insert(200);
    m_root.Insert(1);
    m_root.Insert(400);
    m_root.CopyTree2TreeP();
    m_root.FixSpacesCountTreeP();
    m_root.Print();
    return 0;
}
