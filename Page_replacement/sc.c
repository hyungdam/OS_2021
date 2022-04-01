#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int count_times();
void second_chance();
typedef struct Node {
    int data;
    int ref_bit;
} node;
int page_fault(node[], int);
char buf[200];
int frame_num = 0;
int times = 0;
int sc_fault = 0;

int main(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "check your input.\n");
        exit(1);
    }

    char* file_name = argv[1];
    FILE* fp;

    if((fp = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "fopen err %s\n", file_name);
        exit(1);
    }   
    frame_num = atoi(fgets(buf, sizeof(buf), fp));
    fgets(buf, sizeof(buf), fp);
    times = count_times(buf);
    printf("Used method : Second-Chance\n");
    printf("page reference string : %s\n", buf);
    printf("\tframe   ");
    for(int i = 1; i <= frame_num; i++) {
        printf("%d\t", i);
        if(i == frame_num)
            printf("page fault\n");
    }
    printf("time\n");
    second_chance();
    printf("Number of page faults : %d times\n", sc_fault);
}

int count_times() {
    int cnt = 1;
    for(int i = 0; i < strlen(buf) - 1; i++) {
        if(buf[i] == ' ') {
            if(i == strlen(buf) - 2) {
                continue;
            }
            cnt++;
        }
    }
    return cnt;
}

void second_chance() {
    node queue[frame_num];
    int f_index = 0;
    char* page_ref;
    for(int i = 0; i < frame_num; i++) {
        queue[i].data = -1;
        queue[i].ref_bit = -1;
    }
    for(int i = 0; i < times; i++) {
        if(i == 0)
            page_ref = strtok(buf, " ");
        else
            page_ref = strtok(NULL, " ");
        printf("%d\t\t", i + 1);

        if(page_fault(queue, atoi(page_ref))) {
            sc_fault++;
            // 큐가 비어있을 때
            if(queue[f_index].data == -1) {
                queue[f_index].data = atoi(page_ref);
                queue[f_index].ref_bit = 0;
                for(int j = 0; j < frame_num; j++) {
                    if(queue[j].data == -1)
                        printf(" \t");
                    else
                        printf("%d\t", queue[j].data);
                }
                printf("F\n");
                f_index = (f_index + 1) % frame_num;
            }
            // 큐가 찼을 때
            else {                
                while(1) {
                    if(queue[f_index].ref_bit == 1) {
                        queue[f_index].ref_bit = 0;
                        f_index = (f_index + 1) % frame_num;
                    }
                    else {
                        queue[f_index].data = atoi(page_ref);
                        f_index = (f_index + 1) % frame_num;
                        break;
                    }
                }
                for(int j = 0; j < frame_num; j++) {
                    if(queue[j].data == -1)
                        printf(" \t");
                    else
                        printf("%d\t", queue[j].data);
                }
                printf("F\n");
            }
        }
        else {
            for(int j = 0; j < frame_num; j++) {
                if(queue[j].data == atoi(page_ref)) {
                    queue[j].ref_bit = 1;
                    break;
                }
            }
            for(int j = 0; j < frame_num; j++) {
                if(queue[j].data == -1)
                    printf(" \t");
                else
                    printf("%d\t", queue[j].data);
            }
            printf("\n");
        }    
    }                    
}

int page_fault(node q[], int page_ref) {
    int flag = 1;
    for(int i = 0; i < frame_num; i++) {
        if(q[i].data == page_ref)
            flag = 0;
    }
    return flag;
}
