//
//  main.cpp
//  testASTAR_Maze
//
//  Created by tombp on 20/03/2018.
//  Copyright © 2018 tombp. All rights reserved.
//

#include <iostream>

#define STARTNODE   1
#define ENDNODE     2
#define BARRIER     3

void m_print2d(int A[][10],int r,int c)
{
    printf("\r\n");
    for (int i = 0; i< r; i++) {
        for (int j = 0; j < c; j++)
            printf("%3d",A[i][j]);
        printf("\r\n");
    }
}


typedef struct AStarNode{
    int s_x;
    int s_y;
    int s_g;
    int s_h;
    int s_style;
    AStarNode* s_parent;
    int s_is_in_opentable;
    int s_is_in_closetable;
}AStarNode, *pAStarNode;

void m_print_table_node(pAStarNode pNode,int i,char* s)
{
    printf("%s-> [<%2d > X_Y(%2d,%2d) G_H_F(%3d,%3d,--> %3d)]\r\n",s,i,pNode->s_x,pNode->s_y,pNode->s_g,pNode->s_h,pNode->s_g+pNode->s_h);
    
}



AStarNode map_maze[10][10];

pAStarNode table_open[100];
pAStarNode table_close[100];

int count_table_open = 0;
int count_table_close = 0;

pAStarNode path_stack[100];

pAStarNode debug_stack[100];

pAStarNode static_p_curr_node = NULL;
int top = -1;
void swap_node(int index1,int index2)
{
    pAStarNode tmp;
    tmp = table_open[index1];
    table_open[index1] = table_open[index2];
    table_open[index2] = tmp;
}
bool is_same_node(pAStarNode pNodeA,pAStarNode pNodeB)
{
    return (pNodeA->s_x == pNodeB->s_x) && (pNodeA->s_y == pNodeB->s_y);
}

void adjust_to_min_heap(pAStarNode* p_open,int i,int count)
{
    if (i < 0 || i >= count)
        return;
    pAStarNode tmp;
    int j ,curr ;
    curr = i;
    tmp = p_open[curr];
    j = 2*curr+1;
    while (j < count) {
        if (j+1 < count && (p_open[j+1]->s_g+p_open[j+1]->s_h) < (p_open[j]->s_g+p_open[j]->s_h) )
            j++;
        if ((p_open[curr]->s_g+p_open[curr]->s_h) < (p_open[j]->s_g+p_open[j]->s_h) ) {
            break;
        }
        p_open[curr] = p_open[j];
        p_open[j] = tmp;
        curr = j;
        j = 2*curr+1;
    }
    if (curr != i) {
        return;
    }
    int parent = (curr-1)/2;
    while (curr != 0) {
        if (p_open[parent]->s_g+p_open[parent]->s_h < p_open[curr]->s_g+p_open[curr]->s_h) {
            swap_node(parent, curr);
            curr = parent;
            parent = (curr-1)/2;
        }
    }
    return;
//    p_open[i] = tmp;
}
bool is_x_y_valid(int x,int y)
{
    return (x >= 0 && x < 10 && y >= 0 && y < 10);
}
void insert_to_opentable(int x,int y,pAStarNode curr_node,pAStarNode end_node,int w)
{
    int i;
    if (map_maze[x][y].s_style != BARRIER) {
        if (!map_maze[x][y].s_is_in_closetable) {
            if (map_maze[x][y].s_is_in_opentable) {
                if (map_maze[x][y].s_g > curr_node->s_g + w) {
                    map_maze[x][y].s_g = curr_node->s_g + w;
                    map_maze[x][y].s_parent = curr_node;
                    for (i = 0; i < count_table_open; i++) {
                        if (table_open[i]->s_x == map_maze[x][y].s_x && table_open[i]->s_y == map_maze[x][y].s_y) {
                            break;
                        }
                    }
                    adjust_to_min_heap(table_open, i, count_table_open);
                }
            }
            else{
                map_maze[x][y].s_g = curr_node->s_g + w;
                map_maze[x][y].s_h = 10*abs(map_maze[x][y].s_x - end_node->s_x) + 10*abs(map_maze[x][y].s_y - end_node->s_y );
                map_maze[x][y].s_parent = curr_node;
                map_maze[x][y].s_is_in_opentable = 1;
                table_open[count_table_open++] = &(map_maze[x][y]);
            }
        }
    }
}

void m_print_table_open()
{
    for (int i = 0; i < count_table_open; i++) {
        m_print_table_node(table_open[i], i, "OPEN");
    }
    printf("\r\n");
}
void m_print_track_open(int index)
{
    int top = -1;
    pAStarNode p_curr;
    p_curr = table_open[index];
    while (p_curr) {
        debug_stack[++top] = p_curr;
        p_curr = p_curr->s_parent;
//        printf("S top:%d\r\n",top);
    }
    while (top >=0) {
//        printf("B top:%d\r\n",top);
        m_print_table_node(debug_stack[top--], 0, " Down ");
//        printf("A top:%d\r\n",top);
    }
//    printf("track open");
    printf("\r\n");
}
void m_print_track_close(int index)
{
    int top = -1;
    pAStarNode p_curr;
    p_curr = table_close[index];
    while (p_curr) {
        debug_stack[++top] = p_curr;
        p_curr = p_curr->s_parent;
//        printf("S top:%d",top);
    }
    while (top >=0) {
//        printf("B top:%d",top);
        m_print_table_node(debug_stack[top--], 0, " Down ");
//        printf("A top:%d",top);
    }
    printf("\r\n");
}


void m_print_table_close()
{
    for (int i = 0; i < count_table_close; i++) {
        m_print_table_node(table_close[i], i, "    CLOSE");
    }
    printf("\r\n");
}
void m_print_curr()
{
    m_print_table_node(static_p_curr_node, 0, "CURRENT");
}

void get_neighbors(pAStarNode curr_node,pAStarNode end_node)
{
    int x = curr_node->s_x;
    int y = curr_node->s_y;
    
    if (is_x_y_valid(x+1, y))   // Center South
        insert_to_opentable(x+1, y, curr_node, end_node, 10);
    if (is_x_y_valid(x-1, y))  //  Center North
        insert_to_opentable(x-1, y, curr_node, end_node, 10);
    if (is_x_y_valid(x, y+1))   // Center West
        insert_to_opentable(x, y+1, curr_node, end_node, 10);
    if (is_x_y_valid(x, y-1))   // Center East
        insert_to_opentable(x, y-1, curr_node, end_node, 10);
    if (is_x_y_valid(x+1, y+1)) // South West
        insert_to_opentable(x+1, y+1, curr_node, end_node, 14);
    if (is_x_y_valid(x+1, y-1))  //South East
        insert_to_opentable(x+1, y-1, curr_node, end_node, 14);
    if (is_x_y_valid(x-1, y+1))  //North West
        insert_to_opentable(x-1, y+1, curr_node, end_node, 14);
    if (is_x_y_valid(x-1, y-1))  //North East
        insert_to_opentable(x-1, y-1, curr_node, end_node, 14);
}





int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
//
    pAStarNode p_start_node = NULL;
    pAStarNode p_end_node = NULL;
    pAStarNode p_curr_node = NULL;
    int is_found;
    int maze[][10] = {           // 仅仅为了好赋值给map_maze
        { 1,0,0,0,0,3,0,0,0,0 },
        { 0,0,3,3,0,3,0,3,0,3 },
        { 3,0,0,0,0,3,3,3,0,3 },
        { 3,0,3,0,0,0,0,0,0,3 },
        { 3,0,3,0,0,3,0,0,0,3 },
        { 3,3,3,3,3,3,0,3,0,3 },
        { 3,0,0,0,0,3,3,0,0,0 },
        { 0,0,2,0,0,0,0,0,0,0 },
        { 3,3,3,0,0,3,0,3,0,3 },
        { 3,0,0,0,0,3,3,3,0,3 },
    };
    for (int i = 0; i < 10; i++) {
        for (int j = 0;  j < 10; j++) {
            map_maze[i][j].s_x = i;
            map_maze[i][j].s_y = j;
            map_maze[i][j].s_style = maze[i][j];
            map_maze[i][j].s_g = 0;
            map_maze[i][j].s_h = 0;
            map_maze[i][j].s_parent = NULL;
            map_maze[i][j].s_is_in_opentable = 0;
            map_maze[i][j].s_is_in_closetable = 0;
            if (map_maze[i][j].s_style == STARTNODE) {
                p_start_node = &map_maze[i][j];
            }
            if (map_maze[i][j].s_style == ENDNODE) {
                p_end_node = &map_maze[i][j];
            }
        }
    }
    table_open[count_table_open++] = p_start_node;
    p_start_node->s_g = 0;
    p_start_node->s_h = 10*abs(p_end_node->s_x - p_start_node->s_x) + 10*abs(p_end_node->s_y - p_end_node-> s_y);
    p_start_node->s_is_in_opentable = 1;
    p_start_node->s_parent = NULL;
    if (is_same_node(p_start_node, p_end_node)) {
        printf("start point == end point \r\n");
        return 0;
    }
    is_found = 0;
    while (1) {
        
        for (int j = 0; j < count_table_open; j++) {
            m_print_table_node(table_open[j], j,"OPEN");
        }
        printf("\r\n\r\n");
        p_curr_node = table_open[0];
        static_p_curr_node = p_curr_node;
        table_open[0] = table_open[--count_table_open];
        adjust_to_min_heap(table_open, 0, count_table_open);
        
        table_close[count_table_close++] = p_curr_node;
        p_curr_node->s_is_in_closetable = 1;
        
        for (int j = 0; j < count_table_close; j++) {
            m_print_table_node(table_close[j], j,"   CLOSE");
        }
        
        if (is_same_node(p_curr_node, p_end_node)) {
            is_found = 1;
            break;
        }
        get_neighbors(p_curr_node, p_end_node);
        if (count_table_open == 0) {
            is_found = 0;
            break;
        }
    }
    
    if (is_found) {
        p_curr_node = p_end_node;
        static_p_curr_node = p_curr_node;
        while (p_curr_node) {
            path_stack[++top] = p_curr_node;
            p_curr_node = p_curr_node->s_parent;
            static_p_curr_node = p_curr_node;
        }
        while (top >= 0) {
            if (top > 0) {
                printf("(%d,%d)-->",path_stack[top]->s_x,path_stack[top--]->s_y);
            }else{
                printf("(%d,%d)",path_stack[top]->s_x,path_stack[top--]->s_y);
            }
        }
    }else{
        printf("failed to find out path \r\n");
    }
    
    puts(" ");
    
//    adjust_to_min_heap(table_open, 0, count_table_open);
//    p_curr_node = table_open[0];
    
    
    
    
    return 0;
}
