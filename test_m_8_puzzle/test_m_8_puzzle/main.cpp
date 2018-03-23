//
//  main.cpp
//  test_m_8_puzzle
//
//  Created by tombp on 22/03/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#include <iostream>


int table_map[9] = { 4, 0, 1 , 2 , 5 , 8, 7, 6, 3  }   ;
//with the map_table the all we do is the number after map_table,
//so the target will be the order after map table

void m_print_9_figure(int *A)
{
    for (int i = 0 ; i < 9; i++) {
        printf("%3d\t",A[i]);
        if (i%3==2) {
            printf("\r\n");
        }
    }
}

typedef struct AStarNode{
    int s_nine_arr[9];
    int s_g;
    int s_h;
    int s_0_index;
    int s_is_in_open_table;
    int s_is_in_close_table;
    AStarNode *s_parent;      //record the last status change to current parent pointer
    AStarNode *s_pre;         //record in the list pointer
    AStarNode *s_next;        //record in the list pointer
    
}AStarNode, *pAStarNode ;

void m_print_9_figure_node(pAStarNode pNode)
{
    printf("[\r\n");
    if (pNode != NULL) {
        printf("Node  Add_ME:0x%016llx\r\n",*(uint64_t*)&pNode);
        printf("    Node Add_PRE:0x%016llx\r\n",*(uint64_t*)&(pNode->s_pre));
        printf("        NodeAdd_NEXT:0x%016llx\r\n",*(uint64_t*)&(pNode->s_next));
        printf("G_H_F:(%3d,%3d = %3d ) \r\n",pNode->s_g,pNode->s_h,pNode->s_g + pNode->s_h);
        m_print_9_figure(pNode->s_nine_arr);
    }else{
        printf("NodeAdd: NULL \r\n");
    }
    printf("]\r\n");
}
void m_print_9_figure_node_full(pAStarNode pNode,int bAll)
{
    printf("{\r\n");
    pAStarNode p = pNode;
    while (p!=NULL ) {
        m_print_9_figure_node(p);
        p = p->s_next;
        if (bAll == 0 ) {
            break;
        }
    }
    printf("}\r\n");
    
}

pAStarNode list_open = NULL;
pAStarNode list_close = NULL;
pAStarNode list_stack = NULL;
pAStarNode list_del_from_open = NULL;
pAStarNode path_stack[1000];
bool is_get_target(pAStarNode pNode)
{
    for (int i = 0; i < 9; i++) {
        if (pNode->s_nine_arr[i] != i ) {
            return false;
        }
    }
    return true;
}
int get_0_from_current_node(pAStarNode pNode)
{
    int index = 0 ;
    for (int i = 0 ; i < 9 ; i++) {
        if (pNode->s_nine_arr[i] == 0) {
            index = i;
            break;
        }
    }
    return index;
}

int get_h_from_current_node(pAStarNode pNode)
{
    int cost = 0 ;
    for (int i = 0 ; i < 9 ; i++) {
        if (pNode->s_nine_arr[i] != i) {
            cost++;
        }
    }
    return cost;
}
int get_f_from_current_node(pAStarNode pNode)
{
    return (pNode->s_g + pNode->s_h);
}

pAStarNode get_min_f_from_open_list(pAStarNode pOpen)
{
    pAStarNode p;
    pAStarNode p_min;
    int cost_min = get_f_from_current_node(pOpen);
    p = pOpen;
    p_min = p;
    while (p != NULL) {
        if (get_f_from_current_node(p) < cost_min) {
            cost_min = get_f_from_current_node(p) ;
            p_min = p;
        }
        p = p->s_next;
    }
    return p_min;
}

void list_add_node(pAStarNode* plist ,pAStarNode pNode)
{
    if (pNode == NULL) {
        return;
    }
    if (*plist == NULL) {
        *plist = pNode;
        (*plist)->s_next = NULL;
    }else{
        pNode->s_next = *plist;
        (*plist)->s_pre = pNode;
        *plist = pNode;
    }
    return ;
}

void list_add_to_open_list(pAStarNode* pOpen, pAStarNode pClose,pAStarNode pNode)
{
    pAStarNode p;
    p = pClose;
    int is_in_close_list = 0;
    if (pClose != NULL && pNode != NULL) {
        while (p != NULL) {
            if (memcmp(p->s_nine_arr, pNode->s_nine_arr, sizeof(pNode->s_nine_arr)) == 0) {
                is_in_close_list = 1;
                break;
            }
            p = p->s_next;
        }
    }
    if (is_in_close_list == 0) {
        list_add_node(pOpen, pNode);
    }
    return;
}


pAStarNode move_pNode_from_open_list(pAStarNode* p_Open,pAStarNode pNode,pAStarNode* p_Del)
{
    pAStarNode p_result = NULL;
    pAStarNode p = *p_Open;
    while (p != NULL) {
        if ( p==pNode ) {
            p_result = p;
            break;
        }
        p = p->s_next;
    }
    if (p_result != NULL ) {
        if (*p_Del == NULL) {
            *p_Del = p_result;
            if (p_result == *p_Open) {
                *p_Open = p_result->s_next;
                if (*p_Open != NULL)
                    (*p_Open)->s_pre = NULL;
            }else{
                p_result->s_pre->s_next = p_result->s_next;
                if (p_result->s_next != NULL)
                    p_result->s_next->s_pre = p_result->s_pre;
            }
            p_result->s_next = NULL;
        }else{
            if (p_result ==*p_Open) {
                *p_Open = p_result->s_next;
                if (*p_Open != NULL)
                    (*p_Open)->s_pre = NULL ;
            }else{
                p_result->s_pre->s_next = p_result->s_next ;
                if (p_result->s_next != NULL )
                    p_result->s_next->s_pre = p_result->s_pre;
            }
            p_result->s_next = *p_Del;
            (*p_Del)->s_pre = p_result;
            p_result->s_pre = NULL;
            *p_Del = p_result;
        }
    }
    return p_result;
}

pAStarNode create_neighbors_node(pAStarNode pCurr,int index_0,int index_neighbor)
{
    pAStarNode pNode;
    int nfigure ;
    pNode = (pAStarNode)malloc(sizeof(AStarNode));
    memcpy(pNode->s_nine_arr, pCurr->s_nine_arr, sizeof(pCurr->s_nine_arr));
    nfigure = pNode->s_nine_arr[index_0];
    pNode->s_nine_arr[index_0] = pNode->s_nine_arr[index_neighbor] ;
    pNode->s_nine_arr[index_neighbor] = nfigure ;
    pNode->s_g = pCurr->s_g + 1;
    pNode->s_h = get_h_from_current_node(pNode);
    pNode->s_is_in_open_table = 1;
    pNode->s_is_in_close_table = 0;
    pNode->s_0_index = index_neighbor ;
    pNode->s_parent = pCurr;
    pNode->s_pre = NULL;
    pNode->s_next = NULL;
    return pNode;
}



void get_neighbors(pAStarNode* pOpen,pAStarNode pCurr)
{
    pAStarNode pNode;
    int index_0 = get_0_from_current_node(pCurr);
    switch (index_0) {
        case 0:
            pNode = create_neighbors_node(pCurr, 0, 1);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 0, 3);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 2:
            pNode = create_neighbors_node(pCurr, 2, 1);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 2, 5);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 6:
            pNode = create_neighbors_node(pCurr, 6, 3);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 6, 7);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 8:
            pNode = create_neighbors_node(pCurr, 8, 5);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 8, 7);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 1:
            pNode = create_neighbors_node(pCurr, 1, 0);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 1, 2);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 1, 4);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 3:
            pNode = create_neighbors_node(pCurr, 3, 0);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 3, 6);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 3, 4);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 5:
            pNode = create_neighbors_node(pCurr, 5, 2);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 5, 8);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 5, 4);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 7:
            pNode = create_neighbors_node(pCurr, 7, 6);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 7, 8);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 7, 4);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        case 4:
            pNode = create_neighbors_node(pCurr, 4, 1);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 4, 3);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 4, 5);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            pNode = create_neighbors_node(pCurr, 4, 7);
            list_add_to_open_list(&list_open,list_close, pNode);
            pNode = NULL;
            break;
        default:
            break;
    }
    return;
}

int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
    int is_found;
    int Arr_After_Map[9];
    for (int i= 0; i < 9; i++) {
        Arr_After_Map[table_map[i]] = i;
    }
    int Arr_Start[9] = {  1 , 2 , 3 ,4 ,5 ,6 , 7, 8 , 0 };
    int Arr_Start_Map[9];
    
    for (int i = 0; i < 9; i++) {
        Arr_Start_Map[i] = table_map[i];
    }
    
    m_print_9_figure(Arr_Start_Map);
    
    //add to open list
    
    list_open = (pAStarNode)malloc(sizeof(AStarNode));
    memcpy(list_open->s_nine_arr, Arr_Start_Map, sizeof(Arr_Start_Map));
    list_open->s_parent = NULL;
    list_open->s_pre = NULL;
    list_open->s_next = NULL;
    list_open->s_g = 0;
    list_open->s_0_index = get_0_from_current_node(list_open);
    list_open->s_is_in_open_table = 1;
    
    if (is_get_target(list_open)) {
        printf("start nine figure is the same as end nine figure \r\n");
        return 0;
    }
    is_found = 0;
    
    while (1) {
        pAStarNode pCurr = get_min_f_from_open_list(list_open);
        if (pCurr == NULL) {
            printf("failed to process\r\n");
            break ;
        }
        pCurr = move_pNode_from_open_list(&list_open, pCurr, &list_del_from_open);
        
        pCurr->s_is_in_close_table = 1;
        list_add_node(&list_close, pCurr);
        
        if (is_get_target(pCurr)) {
            is_found = 1;
            break;
        }
        get_neighbors(&list_open,pCurr);
    }

    if (is_found == 1) {
        pAStarNode p;
        p = list_close;
        int top = -1;
        while (p != NULL) {
            path_stack[++top] = p;
            p = p->s_parent;
        }
        while (top >= 0) {
            m_print_9_figure(path_stack[top--]->s_nine_arr);
            printf("\r\n\r\n");
        }
    }
    
    
    
    
//    m_print_9_figure(Arr_After_Map);
    
    return 0;
}
