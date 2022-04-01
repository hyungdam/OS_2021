#include <stdio.h>
#include <linux/kernel.h>
#include <syscall.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char text[20];
void lex(int*, int*, int*);

int main() {
    int res;
    int op1, op2;
    int cal;
    
    lex(&op1, &op2, &cal);
    if(cal == 0) {
        syscall(442, op1, op2, &res);
	    printf("%d + %d = %d\n", op1, op2, res);
        return 0;
    }
    if(cal == 1) {
        syscall(443, op1, op2, &res); 
        printf("%d - %d = %d\n", op1, op2, res);
        return 0;
    }
    if(cal == 2) {
        syscall(444, op1, op2, &res);
        printf("%d * %d = %d\n", op1, op2, res); 
        return 0;
    }
    if(cal == 3) {
        if(syscall(445, op1, op2, &res) == -1) {
            fprintf(stderr, "op2 can't be 0\n");
            return -1;
        }
        printf("%d %% %d = %d\n", op1, op2, res); 
        return 0;
    }        
}

void lex(int* op1, int* op2, int* cal) {
    char ch = ' ';
    int i = 0;
    memset(text, '\0', sizeof(text));
    while (ch == ' ' || ch =='\t')
        ch = getchar();
    if(ch == '-') {
        text[i++] = ch;
        ch = getchar();
    }
    if(isdigit(ch)) {
        do{
            text[i++] = ch;
            ch = getchar();
        } while(isdigit(ch));
        text[i] = 0;
        *op1 = atoi(text);
    }

    while (ch == ' ' || ch =='\t')
        ch = getchar();
    if(ch == '+') {
        ch = getchar();
        *cal = 0;
    }
    else if(ch == '-') {
        ch = getchar();
        *cal = 1;
    }
    else if(ch == '*') {
        ch = getchar();
        *cal = 2;
    }
    else {
        ch = getchar();
        *cal = 3;
    }

    i = 0;
    memset(text, '\0', sizeof(text));
    while (ch == ' ' || ch =='\t')
        ch = getchar();
    if(ch == '-') {
        text[i++] = ch;
        ch = getchar();
    }
    if(isdigit(ch)) {
        do{
            text[i++] = ch;
            ch = getchar();
        } while(isdigit(ch));
        text[i] = 0;
        *op2 = atoi(text);
    }
}
