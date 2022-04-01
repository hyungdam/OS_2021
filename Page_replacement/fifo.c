#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int count_times();
void fifo(int[]);
int page_fault(int[], int);
char buf[200];
int frame_num = 0;
int times = 0;
int fifo_fault = 0;

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
    times = count_times();
    int frame[frame_num];
    memset(frame, -1, sizeof(frame));
    printf("Used method : FIFO\n");
    printf("page reference string : %s\n", buf);
    printf("\tframe   ");
    for(int i = 1; i <= frame_num; i++) {
        printf("%d\t", i);
        if(i == frame_num)
            printf("page fault\n");
    }
    printf("time\n");
    fifo(frame);
    printf("Number of page faults : %d times\n", fifo_fault);
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

void fifo(int frame[]) {
    int f_index = 0;
    char* page_ref;
    for(int i = 0; i < times; i++) {
        if(i == 0)
            page_ref = strtok(buf, " ");
        else
            page_ref = strtok(NULL, " ");
        printf("%d\t\t", i + 1);
        
        if(page_fault(frame, atoi(page_ref))) {
            fifo_fault++;
            frame[f_index] = atoi(page_ref);
            for(int j = 0; j < frame_num; j++) {
                if(frame[j] == -1)
                    printf(" \t");
                else
                    printf("%d\t", frame[j]);
            }
            printf("F\n");
            f_index = (f_index + 1) % frame_num;
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

int page_fault(int frame[], int page_ref) {
    int flag = 1;
    for(int i = 0; i < frame_num; i++) {
        if(frame[i] == page_ref)
            flag = 0;
    }
    return flag;
}
