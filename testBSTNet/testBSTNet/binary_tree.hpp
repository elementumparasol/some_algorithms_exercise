//
//  binary_tree.hpp
//  testBSTNet
//
//  Created by tombp on 03/05/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#ifndef binary_tree_hpp
#define binary_tree_hpp

#include <stdio.h>

//binary_tree.h
struct binary_tree_node{
    int level;
    int value;
    int pre_space;
    binary_tree_node* p_left;
    binary_tree_node* p_right;
};

struct bst_node{
    int value;
    struct bst_node* l;
    struct bst_node* r;
};

binary_tree_node* copy_binary_tree(bst_node* p_bst_root);
void copy_binary_tree_recursively(binary_tree_node* p_node, bst_node* p_bts_node);

binary_tree_node* create_binary_tree_node(int value);
void destroy_binary_tree(binary_tree_node* p_root);

binary_tree_node* create_binary_tree_auto();
void create_binary_tree_recursively(binary_tree_node* p_node);//generate two son nodes, when pass into one node

int get_random_num();

#endif /* binary_tree_hpp */
