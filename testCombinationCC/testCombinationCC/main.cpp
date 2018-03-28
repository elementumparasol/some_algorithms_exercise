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


int main()
{
    printf("AA\r\n");
    int n = NN,k =KK;
    int *x = (int*)malloc(sizeof(int)*n);
    for (int i=0; i <= n; i++) {
        x[i] = 0;
    }
    combine(n, k, x);
    return 0;
}
