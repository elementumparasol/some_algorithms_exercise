//
//  binary_tree.cpp
//  testBSTNet
//
//  Created by tombp on 03/05/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#include "binary_tree.hpp"

//binary_tree.cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int g_random_index=0;

binary_tree_node* create_binary_tree_node(int value,int level){
    binary_tree_node* p_node = new binary_tree_node();
    p_node->value = value;
    p_node->level = level;
    p_node->pre_space = 0;
    p_node->p_left = NULL;
    p_node->p_right = NULL;
    
    return p_node;
}

void destroy_binary_tree(binary_tree_node* p_root)
{
    if(p_root != NULL)
    {
        binary_tree_node* p_left = p_root->p_left;
        binary_tree_node* p_right = p_root->p_right;
        
        delete p_root;
        p_root = NULL;
        
        destroy_binary_tree(p_left);
        destroy_binary_tree(p_right);
    }
}

int get_random_num(){
    if(g_random_index < 1){
        srand((int)time(NULL));
        g_random_index ++;
    }
    int number = (int)(rand()%321);
    return number;
}

//根据一个标准的二叉树来复制打印的树结构
binary_tree_node* copy_binary_tree(bst_node* p_bst_root){
    int value = p_bst_root->value;
    binary_tree_node* p_root = create_binary_tree_node(value,1);
    copy_binary_tree_recursively(p_root,p_bst_root);
    return p_root;
}

void copy_binary_tree_recursively(binary_tree_node* p_node, bst_node* p_bst_node){
    int level = p_node->level;
    
    if(p_bst_node->l != NULL){//left node is not null
        int value_left = p_bst_node->l->value;
        binary_tree_node* p_left = create_binary_tree_node(value_left,level+1);
        p_node->p_left = p_left;
        copy_binary_tree_recursively(p_left, p_bst_node->l);
    }
    
    if(p_bst_node->r != NULL){
        int value_right = p_bst_node->r->value;
        binary_tree_node* p_right = create_binary_tree_node(value_right,level+1);
        p_node->p_right = p_right;
        copy_binary_tree_recursively(p_right, p_bst_node->r);
    }
}
