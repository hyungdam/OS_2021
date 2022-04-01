#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int count_times();
void lru(int[]);
int page_fault(int);
typedef struct NODE* node_ptr;
typedef struct NODE {
    node_ptr uplink;
    node_ptr downlink;
    int data;
} NODE;

node_ptr top = NULL;
node_ptr bottom = NULL;

char buf[200];
int frame_num = 0;
int times = 0;
int lru_fault = 0;

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
    int frame[frame_num];
    memset(frame, -1, sizeof(frame));
    printf("Used method : LRU\n");
    printf("page reference string : %s\n", buf);
    printf("\tframe   ");
    for(int i = 1; i <= frame_num; i++) {
        printf("%d\t", i);
        if(i == frame_num)
            printf("page fault\n");
    }
    printf("time\n");
    lru(frame);
    printf("Number of page faults : %d times\n", lru_fault);
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

void lru(int frame[]) {
    int dlist_num = 0;
    int f_index = 0;
    char* page_ref;
    for(int i = 0; i < times; i++) {
        if(i == 0)
            page_ref =  strtok(buf, " ");
        else
            page_ref = strtok(NULL, " ");
        printf("%d\t\t", i + 1);
        
        if(page_fault(atoi(page_ref))) {
            lru_fault++;
            if(frame_num == 1) {
                node_ptr tmp = (node_ptr)malloc(sizeof(NODE));
                tmp->downlink = top;
                tmp->uplink = NULL;
                tmp->data = atoi(page_ref);
                top = tmp;
                bottom = tmp;
            }                        
            else if(dlist_num < frame_num) {
                node_ptr tmp = (node_ptr)malloc(sizeof(NODE));
                dlist_num++;
                frame[f_index] = atoi(page_ref);
                tmp->downlink = top;
                tmp->uplink = NULL;

                if(top)
                    top->uplink = tmp;
                tmp->data = atoi(page_ref);
                top = tmp;

                if(!bottom)
                    bottom = tmp;
            }
            else {
                node_ptr tmp = (node_ptr)malloc(sizeof(NODE));
                tmp = bottom;
                
                for(int j = 0; j < frame_num; j++) {
                    if(frame[j] == tmp->data) {
                        frame[j] = atoi(page_ref);
                        break;
                    }
                }
               
                tmp->uplink->downlink = tmp->downlink;
                bottom = tmp->uplink;
                tmp->data = atoi(page_ref);
                tmp->downlink = top;
                tmp->uplink = top -> uplink;
                top->uplink = tmp;
                top = tmp;
            }
        
            f_index++;
            for(int j = 0; j < frame_num; j++) {
                if(frame[j] == -1)
                    printf(" \t");
                else
                    printf("%d\t", frame[j]);
            } 
            printf("F\n");
        }
        else {
            for(int j = 0; j < frame_num; j++) {
                if(frame[j] == -1)
                    printf(" \t");
                else
                    printf("%d\t", frame[j]);
            }
            printf("\n");
        }
    }                    
}

int page_fault(int page_ref) {
    int flag = 1;
    node_ptr tmp = (node_ptr)malloc(sizeof(NODE));
    node_ptr pos = (node_ptr)malloc(sizeof(NODE));
    
    tmp = top;
    pos = top;
    
    while(pos != NULL) {
        if(pos->data == page_ref) {
            tmp = pos;
            flag = 0;
            if((tmp->uplink) && (tmp->downlink)) {
                tmp->uplink->downlink = tmp->downlink;
                tmp->downlink->uplink = tmp->uplink;
                tmp->downlink = top;
                tmp->uplink = top->uplink;
                top->uplink = tmp;
                top = tmp;
            }
            else if(!(tmp->downlink) && (tmp->uplink)) {
                tmp->uplink->downlink = tmp->downlink;
                bottom = tmp->uplink;
                tmp->uplink = top->uplink;
                top->uplink = tmp;
                tmp->downlink = top;
                top = tmp;
            }
        }
        pos = pos->downlink;
    }
    return flag;
}
