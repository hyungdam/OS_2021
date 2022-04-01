#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void matrix_cal(int);

int main() {
    pid_t pid[21];
    int status;
    
    for(int i = 0; i < 21; i++) {
        if(pid[i] = fork() > 0) {
        }
        else if(pid[i] == 0) {
            printf(" %d process begins\n", getpid());
            
            if(i < 7) {
                matrix_cal(10000);
            }
            else if(i < 14) {
                matrix_cal(50000);
            }
            else if(i < 21) {
                matrix_cal(100000);
            }
            exit(0);
        }
    }
    
    for(int i = 0; i < 21; i++) {
        pid_t end_pid = wait(&status);
        printf(" %d process ends\n", end_pid);
    }
    printf("----------- All processes end -----------\n");
    return 0;
}

void matrix_cal(int size) {
    int** arr;
    arr = (int**)malloc(sizeof(int*) * size);
    for(int i = 0; i < size; i++) {
        arr[i] = (int*)malloc(sizeof(int) * size);
    }

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++)
            arr[i][j] = i + j;
    }

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++)
            arr[i][j] += 10;
    }
}
