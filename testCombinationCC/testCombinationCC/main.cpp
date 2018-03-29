#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>



#define NN 4
#define KK 2


bool b_traveled[100];

int sum_x(int *x,int count)
{
    int sum = 0;
    for (int i= 1; i <= count; i++) {
        sum += x[i];
    }
    return sum;
}
void print_solution(int *x,int count)
{
    printf("[ ");
    for (int i= 1; i <= count; i++) {
        if (x[i] == 1) {
            printf("%3d  ",i);
        }
    }
    printf(" ]\r\n");
}

int get_traveled_index(int *x,int t)
{
    int n = 0;
    for (int i = 1; i <= t ; i++) {
        n = 2*n + 1 + (1 - x[i]);
    }
    return n;
}

bool travel_ok(int traveled_index,int t,int count)
{
    bool b = true;
    bool b_already_travel = false;
    if (traveled_index%2 == 0) {   //右孩子
        if ((traveled_index-2)/2 >=2 && (traveled_index-2)/2%2 == 0) {  //父亲也是右孩子
            if (traveled_index > 2<< (count-1)) {  //叶子
                if (b_traveled[traveled_index-1]) {
                    b_already_travel = true;
                }
            }else{
                if ( b_traveled[traveled_index-1] && b_traveled[2*traveled_index+1] && b_traveled[2*traveled_index+2]) { //如果左兄弟和左右孩子都已经标记，则已经不需要遍历
                    b_already_travel = true;
                }
            }
        }else{    //父亲是左孩子
            if (b_traveled[traveled_index])
                b_already_travel = true;
        }
    }else{  //左孩子
        b_already_travel = b_traveled[traveled_index];
    }
    
    if ((traveled_index+2 & traveled_index+1)==0) {  //最右的分支
        if(b_traveled[traveled_index])
            b_already_travel = true;
    }
    b = !b_already_travel;
    return b;
}
bool node_value_need_changed(int *x,int t)
{
    bool b = false;
    int n_travel = get_traveled_index(x, t);
    if (x[t] == 1 && b_traveled[2*n_travel+1] && b_traveled[2*n_travel+2]) {
        b = true;
    }
    return b;
}
void mark_travel_flags(bool *b_flag,int *x,int t,int layers)
{
    int travel_index ;
    travel_index = get_traveled_index(x, t);
    b_flag[travel_index] = true;
    
    if (t >= layers) {
        if (travel_index%2 == 0) {
            if ((travel_index-2)%4 == 0) {
                b_flag[(travel_index-2)/2] = true;
                b_flag[travel_index-1] = true;
            }
        }else{
            if (((travel_index+1+2) & (travel_index+1+1)) == 0){
                for (int i = travel_index+1; i >=2 ; i = (i-2)/2) {
                    b_flag[i] = true;
                }
             }
        }
        if (((travel_index+2) & (travel_index+1)) == 0) {
            for (int i = travel_index; i >=2 ; i = (i-2)/2) {
                b_flag[i] = true;
            }
        }
        
    }
    if ((travel_index-2)%4==0) {
        for (int i = travel_index,count = 0; (i>=2)&& count<1   ; i= (i-2)/2,count++) {
            if (b_flag[travel_index-1]) {
                b_flag[(travel_index-2)/2] = true;
            }
        }
    }
    
    return;
}

void combine(int num,int k_choose,int *x)
{
    int t = 1;
    int traveled_index = 0;
    while (t > 0) {
        traveled_index = get_traveled_index(x, t);
        if ( travel_ok(traveled_index,t,num) && t <= num ) {
            x[t] = 1;
            traveled_index = get_traveled_index(x, t);
            if (b_traveled[traveled_index] || node_value_need_changed(x, t)) {
                x[t] = 0;
            }
            if (sum_x(x, t) <= k_choose) {
                if (sum_x(x, t) == k_choose) {
                    print_solution(x, num);
                    traveled_index = get_traveled_index(x, t);
                    b_traveled[traveled_index] = true;
                    if (t == num - 1) {
                        b_traveled[2*traveled_index+1] = true;
                        b_traveled[2*traveled_index+2] = true;
                        if (traveled_index%2==1 && (traveled_index-1)/2%2 ==1 ) {
                            b_traveled[traveled_index+1] = true;
                        }
                    }
                    x[t] = 0;
                }else{
                    if (t == num) {
                        traveled_index = get_traveled_index(x, t);
                        b_traveled[traveled_index] = true;
                    }
                    if (sum_x(x, num) == 0) {
                        if (t > 1) {
                            traveled_index = get_traveled_index(x, t);
                            b_traveled[traveled_index] = true;
                        }
                    }
                }
                t++;
            }
        }else{
            mark_travel_flags(b_traveled, x, t, num);
            t--;
        }
    }
}

typedef struct S_virtual_node{
    int b_tree_index;
    bool disable;
} S_virtual_node ,*p_m_virtual_node;
S_virtual_node m_virtual_node[100];
int comb_node[100];
int comb_top = -1;
int top = -1;

int get_tree_weight_and_mark(int stack_p,int n,int k)
{
    int sum = 0;
    int p = m_virtual_node[stack_p].b_tree_index;
    while (p>=1) {
        sum = sum + p%2;
        p = (p-1)/2;
    }
    if (sum == k) {
        m_virtual_node[stack_p].disable = true;
    }else{
        if (p > (2<<(n-1))){
            m_virtual_node[stack_p].disable = true;
        }
    }
    return sum;
}



void tree_combination(int n,int k)
{
    comb_top = -1;
    top = -1;
    int p = 0;
    int top_l = 0;
    m_virtual_node[p].disable = false;
    m_virtual_node[p].b_tree_index = 0;
    top = p;   //push
    do{
        p = top--;  //pop;
        top_l = top;
        if (get_tree_weight_and_mark(p,n,k) < k) {
            if ( 2*m_virtual_node[p].b_tree_index+1 < (2<<n) && !m_virtual_node[p].disable ) {
                top++;
                top_l = top;
                
                top++;
                top_l = top;
                m_virtual_node[top].disable = false;
                m_virtual_node[top].b_tree_index = 2 * m_virtual_node[p].b_tree_index + 2;   //right child push
                top++;
                top_l = top;
                m_virtual_node[top].disable = false;
                m_virtual_node[top].b_tree_index = 2 * m_virtual_node[p].b_tree_index + 1;   //left child
            }
        }else{    // equal k
            comb_node[++comb_top] = m_virtual_node[p].b_tree_index;
        }
    }while (top >= 0);

}
int mini_stack[100];
int mini_top = -1;

int get_tree_weight(int node)
{
    int sum = 0;
    int p = node;
    while (p>=1) {
        sum = sum + p%2;
        p = (p-1)/2;
    }
    return sum;
}


void virtual_tree_travel(int n,int k)
{
    mini_top = -1;
    comb_top = -1;
    int p = 0;
    do{
        while(p < ((2<<n)-1)){
            mini_stack[++mini_top] = p;
            p = p*2+1;
        }
        if (mini_top >= 0) {
            p = mini_stack[mini_top--];
            if (get_tree_weight(p) == k && p%2==1) {
                comb_node[++comb_top] = p;
            }
            p = p+1;
        }
    }while (p < ((2<<n)-1) || mini_top >= 0) ;
}


int main()
{
    printf("AA\r\n");
    int n = NN,k =KK;
    int *x = (int*)malloc(sizeof(int)*n);
//    for (int i=0; i <= n; i++) {
//        x[i] = 0;
//    }
//    tree_combination(n, k);
    virtual_tree_travel(n, k);
    return 0;
}
