#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int count_times();
void set_page_ref(int[]);
void opt(int[]);
int page_fault(int[], int);
int next_offset(int[], int, int, int[]);
char buf[200];
int frame_num = 0;
int times = 0;
int opt_fault = 0;

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
    printf("Used method : OPT\n");
    printf("page reference string : %s\n", buf);
    printf("\tframe   ");
    for(int i = 1; i <= frame_num; i++) {
        printf("%d\t", i);
        if(i == frame_num)
            printf("page fault\n");
    }
    printf("time\n");
    opt(frame);
    printf("Number of page faults : %d times\n", opt_fault);
}

int count_times() {
    int cnt = 1;
    for(int i = 0; i < strlen(buf) - 1; i++) {
        if(buf[i] == ' ') {
            if(i == strlen(buf) - 2){
                continue;
            }
            cnt++;
        }
    }
    return cnt;
}

void set_page_ref(int page_ref[]) {
    char* tmp;
    for(int i = 0; i < times; i++) {
        if(i == 0)
            tmp = strtok(buf, " ");
        else    
            tmp = strtok(NULL, " ");
        page_ref[i] = atoi(tmp);
    }
 
}

void opt(int frame[]) {
    int f_index = 0;
    int page_ref_arr[times];
    int offset[frame_num];
    set_page_ref(page_ref_arr);
    for(int i = 0; i < times; i++) {
        printf("%d\t\t", i + 1);
        
        if(page_fault(frame, page_ref_arr[i])) {
            opt_fault++;
            if(frame[f_index] == -1) {
                frame[f_index] = page_ref_arr[i];
                for(int j = 0; j < frame_num; j++) {
                    if(frame[j] == -1)
                        printf(" \t");
                    else
                        printf("%d\t", frame[j]);
                }
                printf("F\n");
                f_index++;
            }                        
            else {
                int target = 0;
                for(int j = 0; j < frame_num; j++) {
                    offset[j] = next_offset(frame, i, j, page_ref_arr);
                }
                for(int j = 0; j < frame_num - 1; j++) {
                    if(offset[target] < offset[j + 1])
                        target = j + 1;
                }
                frame[target] = page_ref_arr[i];
                for(int j = 0; j < frame_num; j++) {
                    if(frame[j] == -1)
                        printf(" \t");
                    else
                        printf("%d\t", frame[j]);
                }
                printf("F\n");
            }            
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

int next_offset(int frame[], int now, int f_index, int page_ref_arr[]) {    
    for(int i = now + 1; i < times; i++) {
        if(frame[f_index] == page_ref_arr[i])
            return i;
    }
    return (times + 1);
}
