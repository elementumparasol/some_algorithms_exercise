//
//  M_RBTree.cpp
//  testBinarySearchTree
//
//  Created by tombp on 10/05/2018.
//  Copyright © 2018 tombp. All rights reserved.
//
#include <iostream>
#include "M_RBTree.hpp"
#include <deque>

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
void set_string_node_pointer(char *p_str_node,char *p_str_node_pointer,M_RBTree_P* p_node_t)
{
    char ch[20];
    char ch_l[20];
    char ch_r[20];
    int ch_pos = p_node_t->pre_space;
    char *p_node = p_str_node;
    char *p_node_pointer = p_str_node_pointer;
    int underline_left_len = 0;
    int underline_left_index = 0;
    int underline_right_len = 0;
    int underline_right_index = 0;
    int vertical_left_index = 0;
    int vertical_left_len = 1;
    int vertical_right_index = 0;
    int vertical_right_len = 1;
    memset(ch, 0, sizeof(ch));
    memset(ch_l, 0, sizeof(ch_l));
    memset(ch_r, 0, sizeof(ch_r));
    sprintf(ch, "%d",p_node_t->key);
    if (!p_node_t->bRed) {
        memcpy(ch+strlen(ch), "'",1);
    }else{
        memcpy(ch+strlen(ch), ".",1);
    }
    memcpy(p_node+ch_pos, ch, strlen(ch));
    if (p_node_t->left) {
        sprintf(ch_l, "%d",p_node_t->left->key);
        underline_left_len = p_node_t->pre_space - p_node_t->left->pre_space - strlen(ch_l) + 1;
        underline_left_index = p_node_t->left->pre_space+strlen(ch_l)-1;
        for (int i = 0; i < underline_left_len; i++) {
            memcpy(p_node+underline_left_index+i, "_", 1);
        }
        memcpy(p_str_node_pointer+underline_left_index, "|", 1);
    }
    if (p_node_t->right) {
        underline_right_len = p_node_t->right->pre_space - p_node_t->pre_space - strlen(ch)+1;
        underline_right_index = p_node_t->pre_space + strlen(ch);
        for (int i = 0; i < underline_right_len; i++) {
            memcpy(p_node+underline_right_index+i, "_", 1);
        }
        memcpy(p_node_pointer+underline_right_index+underline_right_len-1, "|", 1);
    }
    
    
}


M_C_RB_Tree::M_C_RB_Tree()
{
    //    printf("\r\nIA_TestGroup: Constructor .");
    //    m_act_list = LinkedList<DS_ITEM_List>();
    
}
M_C_RB_Tree::~M_C_RB_Tree()
{
    //    printf("\r\nIA_TestGroup: Constructor .");
    //    m_act_list = LinkedList<DS_ITEM_List>();
    
}
void M_C_RB_Tree::Insert(int key)
{
    if (p_root == NULL) {
        p_root = (M_RBTree*)malloc(sizeof(M_RBTree));
        p_root->key = key;
        p_root->left = NULL;
        p_root->right = NULL;
        p_root->p = NULL;
        p_root->bRed = false;
        return;
    }
    M_RBTree *p_ipos,*p;
    p_ipos = p_root;
    p = p_root;
    while (p != NULL) {
        if (key <= p->key) {
            p_ipos = p;
            p = p->left;
        }else{
            p_ipos = p;
            p = p->right;
        }
    }
    p = (M_RBTree*)malloc(sizeof(M_RBTree));
    p->key = key;
    p->left = NULL;
    p->right = NULL;
    p->p = p_ipos;;
    p->bRed = false;
    if (p->key <= p_ipos->key) {
        p_ipos->left = p;
    }else{
        p_ipos->right = p;
    }
}

void M_C_RB_Tree::CopyTree2TreeP()
{
    if (p_root_p == NULL && p_root != NULL) {
        M_RBTree *p_rb;
        M_RBTree_P *p_rbp;
        M_RBTree_P *p_rbp_up;
        p_rb = p_root;
        p_rbp = p_root_p;
        p_rbp_up = p_root_p;
        mini_top = -1;
        bool bRight = false;
        while (p_rb != NULL || mini_top > -1 ) {
            while (p_rb != NULL) {
                p_rbp = (M_RBTree_P*)malloc(sizeof(M_RBTree_P));
                p_rbp->key = p_rb->key;
                p_rbp->bRed = p_rb->bRed;
                p_rbp->left = NULL;
                p_rbp->right = NULL;
                p_rbp->p = NULL;
                p_rbp->pre_space = 0;
                if (p_root_p == NULL) {
                    p_rbp->level = 1;
                    p_root_p = p_rbp;
                }else{
                    p_rbp->level = p_rbp_up->level + 1 ;
                    if (bRight) {
                        p_rbp_up->right = p_rbp;
                        bRight = false;
                    }else{
                        p_rbp_up->left = p_rbp;
                    }
                }
                p_rbp_up = p_rbp;
                RB_mini_stack[++mini_top] = p_rb;
                RBP_mini_stack[mini_top] = p_rbp;
                p_rb = p_rb->left;
            }
            if (mini_top > -1) {
                p_rb = RB_mini_stack[mini_top];
                p_rbp = RBP_mini_stack[mini_top--];
                p_rbp_up = p_rbp;
                p_rb = p_rb->right;
                bRight = true;
            }
        }
        
        
    }
}
int gl_spaces = 0;

void M_C_RB_Tree::FixSpacesCountTreeP()
{
    M_RBTree_P *p;
    int g_spaces = 0;
    p = p_root_p;
    mini_top = -1;
    while (p != NULL || mini_top > -1 ) {
        while (p != NULL) {
            RBP_mini_stack[++mini_top] = p;
            p = p->left;
        }
        if (mini_top > -1) {
            p = RBP_mini_stack[mini_top--];
            p->pre_space = g_spaces++;
            g_spaces += calc_characters(p->key);
            p = p->right;
        }
    }
    gl_spaces = g_spaces;
}

void M_C_RB_Tree::Print()
{
    mini_head = 0;
    mini_tail = -1;
    M_RBTree_P *p;
    p = p_root_p;
    char str_node[200];
    char str_node_pointer[200];
    int current_level = 1;
    memset(str_node, 0, sizeof(str_node));
    memset(str_node_pointer, 0, sizeof(str_node_pointer));
    memset(str_node, ' ', gl_spaces+1);
    memset(str_node_pointer, ' ', gl_spaces+1);
    while (p != NULL || (mini_head <= mini_tail)) {
        if (p->level == current_level) {
            set_string_node_pointer(str_node,str_node_pointer,p);
        }
        if (p->level > current_level ) {
            printf("%s",str_node);
            printf("\r\n");
            printf("%s",str_node_pointer);
            printf("\r\n");
            memset(str_node, ' ', gl_spaces+1);
            memset(str_node_pointer, ' ', gl_spaces+1);
            set_string_node_pointer(str_node,str_node_pointer,p);
            current_level = p->level;
        }
        if (p->left != NULL) {
            RBP_mini_stack[++mini_tail] = p->left;
        }
        if (p->right != NULL) {
            RBP_mini_stack[++mini_tail] = p->right;
        }
        if (mini_head <= mini_tail) {
            p = RBP_mini_stack[mini_head];
        }
        if (mini_head > mini_tail) {
            printf("%s",str_node);
            printf("\r\n");
            printf("%s",str_node_pointer);
            printf("\r\n");
            break;
        }
        mini_head++;
    }
    
    
}


