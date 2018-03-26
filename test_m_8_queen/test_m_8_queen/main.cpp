//
//  main.cpp
//  test_m_8_queen
//
//  Created by tombp on 24/03/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#include <iostream>

#define N 8

int m_queen[N][N];

typedef struct AStarNode{
    int s_queen[N][N];
    int s_step;
    int s_g;
    int s_h;
    AStarNode* s_parent;
    AStarNode* s_pre;
    AStarNode* s_next;
    int s_is_in_open_table;
    int s_is_in_close_table;
}AStarNode,*pAStarNode;

pAStarNode list_open;
pAStarNode list_close;
pAStarNode list_del_from_open;
pAStarNode list_result[1000];
pAStarNode path_stack[1000];

void m_print_queen(pAStarNode p)
{
    if (p == NULL) {
        return ;
    }
    for (int i= 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%3d",p->s_queen[i][j]);
        printf("\r\n");
    }
    printf("\r\n");
}

void m_print_node_queen(pAStarNode p)
{
    printf("[\r\n");
    if (p == NULL){
        printf("NodeAdd:NULL \r\n");
    }else{
        printf("Node Add_ME:0x%016llx\r\n",*(int64_t*)&p);
        printf("    Node Add_PRE:0x%016llx\r\n",*(int64_t*)&(p->s_pre));
        printf("        Node Add_NEXT:0x%016llx\r\n",*(int64_t*)&(p->s_next));
        printf("STEP = %d \r\n",p->s_step);
        m_print_queen(p);
    }
    printf("]\r\n");
}
int tmp_count_node(pAStarNode pNode)
{
    int nodecount = 0;
    pAStarNode p = pNode;
    while (p!= NULL) {
        nodecount++;
//        m_print_node_queen(p);
        p = p->s_next;
    }
    return nodecount;
}
void m_print_node_queen_full(pAStarNode pNode,int bAll)
{
    int nodecount = 0;
    printf("{\r\n");
    pAStarNode p = pNode;
    while (p!= NULL) {
        nodecount++;
        m_print_node_queen(p);
        p = p->s_next;
        if (bAll == 0) {
            break;
        }
    }
    printf("} Total Node :%d \r\n",nodecount);
}


void list_add_node(pAStarNode *plist, pAStarNode pNode)
{
    if (pNode == NULL)
        return;
    if (*plist == NULL) {
        *plist = pNode;
        (*plist)->s_pre = NULL;
        (*plist)->s_next = NULL;
        return;
    }
    pNode->s_next = (*plist);
    (*plist)->s_pre = pNode;
    *plist = pNode;
    (*plist)->s_pre = NULL;
    return;
}
bool is_get_target(pAStarNode pNode)
{
    return (pNode->s_step == N);
}
pAStarNode get_min_f_from_open_list(pAStarNode pOpen)
{
    pAStarNode pMin,p;
    p = pOpen;
    pMin = p;
    while (p != NULL) {
        if (p->s_g < pMin->s_g) {
            pMin = p;
        }
        p = p->s_next;
    }
    return pMin;
}
pAStarNode move_pNode_from_open_list(pAStarNode *p_Open, pAStarNode pNode, pAStarNode* p_Del)
{
    pAStarNode p_result = NULL;
    p_result = *p_Open;
    while (p_result != NULL) {
        if (p_result == pNode) {
            break;
        }
        p_result = p_result->s_next;
    }
    if (p_result != NULL) {
        if (*p_Open == p_result) {
            *p_Open = p_result->s_next;
            if (*p_Open != NULL) {
                (*p_Open)->s_pre = NULL;
            }
        }else{
            p_result->s_pre->s_next = p_result->s_next;
            if (p_result->s_next != NULL) {
                p_result->s_next->s_pre = p_result->s_pre;
            }
        }
        if (*p_Del == NULL) {
            *p_Del = p_result;
            (*p_Del)->s_pre = NULL;
            (*p_Del)->s_next = NULL;
        }else{
            p_result->s_pre = NULL;
            p_result->s_next = (*p_Del);
            (*p_Del)->s_pre = p_result;
            *p_Del = p_result;
        }
    }
    return p_result;
}


typedef struct ArrayNN
{
    int element[N][N];
} ArrayNN,*pArrayNN;

bool pos_is_valid(int** arr,int rowReadyCount,int col)
{
    bool b_result = true;
    pArrayNN p;
    p = (pArrayNN)arr;
    for (int i = 0; i < rowReadyCount; i++) {
        if (p->element[i][col]==1) {
            b_result = false;
        }
    }
    if (b_result == true ) {
        for (int i= 0; i <= rowReadyCount; i++) {
            if (rowReadyCount-i >= 0 && col+i < N) {
                if (p->element[rowReadyCount-i][col+i]==1) {
                    b_result = false;
                    break;
                }
            }
            if (rowReadyCount-i >= 0 && col-i >= 0) {
                if (p->element[rowReadyCount-i][col-i]==1) {
                    b_result = false;
                    break;
                }
            }
        }
    }
    return b_result ;
}


void get_neighbors(pAStarNode* pOpen,pAStarNode pCurr)
{
    pAStarNode pNode;
    int index = 0;
    if (pCurr->s_step == N) {
        return;
    }
    if (pCurr->s_step == 0) {
        for (int i= 0; i < N; i++) {
            pNode = (pAStarNode)malloc(sizeof(AStarNode));
            memset(pNode->s_queen, 0, sizeof(pNode->s_queen));
            pNode->s_parent = NULL;
            pNode->s_pre = NULL;
            pNode->s_next = NULL;
            pNode->s_queen[0][i] = 1;
            pNode->s_g = 1;
            pNode->s_step = 1;
            pNode->s_parent = pCurr;
            list_add_node(&list_open, pNode);
        }
        return ;
    }
    index = pCurr->s_step ;
    for (int i = 0; i < N; i++) {
        if (pos_is_valid((int**)pCurr->s_queen,index,i)) {
            pNode = (pAStarNode)malloc(sizeof(AStarNode));
            pNode->s_parent = NULL;
            pNode->s_pre = NULL;
            pNode->s_next = NULL;
            memcpy(pNode->s_queen, pCurr->s_queen, sizeof(pCurr->s_queen));
            pNode->s_queen[index][i] = 1;
            pNode->s_g = pCurr->s_g + 1;
            pNode->s_step = index + 1;
            pNode->s_parent = pCurr;
            list_add_node(&list_open, pNode);
        }
    }
    
}


int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
    int m_queen[N][N];
    int is_found = 0;
    int top_result = -1;
    for (int i = 0; i< N; i++) {
        for (int j = 0;  j < N; j++) {
            m_queen[i][j] = 0;
        }
    }
    list_open = (pAStarNode)malloc(sizeof(AStarNode));
    memcpy(list_open->s_queen, m_queen, sizeof(m_queen));
    list_open->s_g = 0;
    list_open->s_h = 0;
    list_open->s_pre = NULL;
    list_open->s_next = NULL;
    list_open->s_parent = NULL;
    list_open->s_step = 0;
    list_open->s_is_in_open_table = 1;
    m_print_node_queen_full(list_open, 0);
    if (is_get_target(list_open)) {
        printf("the first create is the target,of couse ,this is impossible\r\n");
        return 0;
    }
    while (1) {
        pAStarNode pCurr;
        pCurr = get_min_f_from_open_list(list_open);
        if (pCurr == NULL) {
            printf("All done\r\n");
            break;
        }
        if (tmp_count_node(list_open)==14) {
            printf("Debug");
        }
        if (pCurr->s_step == 4) {
            printf("Debug");
        }
        pCurr = move_pNode_from_open_list(&list_open, pCurr, &list_close);
        pCurr->s_is_in_close_table = 1;
        if (pCurr->s_step == N) {
            list_result[top_result++] = pCurr;
        }
        get_neighbors(&list_open,pCurr);
        
    }
    pAStarNode p,tmp;
    p = list_close;
    while (p != NULL) {
        tmp = p;
        p = p->s_next;
        free(tmp);
    }
    
    if (top_result < 0) {
        printf("Can not find the result;\r\n");
    }
    printf("Total Found:%d",top_result);
    while (top_result >= 0) {
        m_print_node_queen(list_result[top_result--]);
        printf("\r\n");
    }
    

    return 0;
}
