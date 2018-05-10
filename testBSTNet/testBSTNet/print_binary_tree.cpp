//
//  print_binary_tree.cpp
//  testBSTNet
//
//  Created by tombp on 03/05/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#include "print_binary_tree.hpp"

//print_binary_tree.cpp

#include <stdio.h>
#include "binary_tree.hpp"
#include <deque>
#include <iostream>
#include <string> //处理字符串
#include <string.h> //处理字符串
using namespace std;

int g_spaces = 0;

int calc_characters(int value){
    if(value == 0){
        return 1;
    }
    int characters = 0;
    while(value){
        characters ++;
        value = value/10;
    }
    return characters;
}

struct str_n{
    string str;
    string str_pointer;
};


struct str_n* set_string_node(string str, string str_pointer, binary_tree_node* p_node){
    //set value
    int value_pos = p_node->pre_space;
    char ch[10];
    sprintf(ch,"%d",p_node->value);
    str.replace(value_pos,strlen(ch),ch);
    
    //set vertical line
    string vertical_l;
    string vertical_r;
    
    //set underline left
    int underline_length_left = 0;
    int underline_index_left = 0;
    int vertical_left_length = 0;
    if(p_node->p_left != NULL){
        underline_length_left = p_node->pre_space
        - p_node->p_left->pre_space
        - calc_characters(p_node->p_left->value) + 1;
        underline_index_left = p_node->p_left->pre_space
        + calc_characters(p_node->p_left->value) - 1;
        vertical_left_length = 1;
        vertical_l.assign(1,'|');
    }
    string str_underline_left(underline_length_left,'_');
    str.replace(underline_index_left,underline_length_left,str_underline_left);
    str_pointer.replace(underline_index_left,vertical_left_length,vertical_l);
    
    //set underline right
    int underline_length_right = 0;
    int underline_index_right = 0;
    int vertical_right_length = 0;
    int vertical_right_index = 0;
    if(p_node->p_right != NULL){
        underline_length_right = p_node->p_right->pre_space
        - p_node->pre_space
        -calc_characters(p_node->value) + 1;
        underline_index_right = p_node->pre_space + calc_characters(p_node->value);
        vertical_right_length = 1;
        vertical_right_index = underline_index_right+underline_length_right-1;
        vertical_r.assign(1,'|');
    }
    string str_underline_right(underline_length_right,'_');
    str.replace(underline_index_right,underline_length_right,str_underline_right);
    str_pointer.replace(vertical_right_index,vertical_right_length,vertical_r);
    
    struct str_n* p_str_n = new str_n();
    p_str_n->str = str;
    p_str_n->str_pointer = str_pointer;
    
    return p_str_n;
}

int g_currunt_level = 1;

void print_from_top_to_bottom(binary_tree_node* p_root)
{
    if(p_root == NULL)
        return;
    
    //队列
    std::deque<binary_tree_node *> deque_tree_node;
    
    deque_tree_node.push_back(p_root);
    
    string str_node(g_spaces+1,' ');
    string str_node_pointer(g_spaces+1,' ');
    //cout << string << endl;
    while(deque_tree_node.size())
    {
        binary_tree_node* p_node = deque_tree_node.front();//从前面获取结点
        deque_tree_node.pop_front();//之后将其弹出
        
        if(p_node->level == g_currunt_level){
            struct str_n* p_str_n = set_string_node(str_node, str_node_pointer, p_node);
            str_node = p_str_n->str;
            str_node_pointer = p_str_n->str_pointer;
        }
        if(p_node->level > g_currunt_level){
            //output str_node and then set str_node to empty
            cout << str_node << endl;
            cout << str_node_pointer << endl;
            str_node.assign(g_spaces+1,' ');
            str_node_pointer.assign(g_spaces+1,' ');
            g_currunt_level = p_node->level;
            struct str_n* p_str_n = set_string_node(str_node, str_node_pointer, p_node);
            str_node = p_str_n->str;
            str_node_pointer = p_str_n->str_pointer;
        }
        
        //do_print_binary_tree(p_node);
        
        //先压入左结点
        if(p_node->p_left)
            deque_tree_node.push_back(p_node->p_left);
        
        //后压入右结点
        if(p_node->p_right)
            deque_tree_node.push_back(p_node->p_right);
    }
    cout << str_node << endl;
    g_currunt_level = 1;//这个很重要，之前没有设置害的我只能打印一次
    g_spaces = 0;
    printf("\n");
}

void in_order(binary_tree_node* p_node){
    if(p_node->p_left!=NULL){
        in_order(p_node->p_left);
    }
    //Do Something with root
    p_node->pre_space = g_spaces;
    g_spaces += calc_characters(p_node->value);
    //printf("%d(%d)\t",p_node->value,p_node->pre_space);
    if(p_node->p_right!=NULL){
        in_order(p_node->p_right);
    }
}

void print_binary_tree(bst_node* p_bst_root){
    struct binary_tree_node* p_root = copy_binary_tree(p_bst_root);
    in_order(p_root);
    print_from_top_to_bottom(p_root);
}
