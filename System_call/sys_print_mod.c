#include <linux/kernel.h>
#include <linux/syscalls.h>

asmlinkage int sys_print_mod(int op1, int op2, int* res) {
    if (op2 == 0) 
		return -1;
	int val;
	val = op1 % op2;
	put_user(val, res);
    return 0;
}

SYSCALL_DEFINE3(print_mod, int, op1, int, op2, int*, res) {
	return sys_print_mod(op1, op2, res);
}

