#include <linux/kernel.h>
#include <linux/syscalls.h>

asmlinkage int sys_print_mul(int op1, int op2, int* res) {
    int val;
	val = op1 * op2;
	put_user(val, res);
	return 0;
}

SYSCALL_DEFINE3(print_mul, int, op1, int, op2, int*, res) {
    return sys_print_mul(op1, op2, res);
}

