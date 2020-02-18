#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//This call creates a new kernel thread which shares the calling process's address space
int
sys_clone(void) {
	void *arg1, *arg2, *stack, *fcn;
	
	if(argptr(0, (char **)&fcn, 4) < 0
		|| argptr(1, (char **)&arg1, 4) < 0
		|| argptr(2, (char **)&arg2, 4) < 0
		|| argptr(3, (char **)&stack, 4) < 0)
		return -1;
	
//	cprintf("inside sys_clone %d\n", (uint)stack);

	//check the stack is page aligned
	if ((uint)stack % PGSIZE != 0)
		return -1;
	
	//check if the address space is less than one page 
	if (proc->sz - (uint)stack < PGSIZE)
		return -1;
	//call the helper method in proc.c
	return cloneHelper(fcn, arg1, arg2, stack);
}

//This system call waits for a child thread that shares the address space with the 
//calling process.  It returns the PID of waited-for child or -1 if none. 
int
sys_join(void) {
	void **stack = NULL;
	if (argptr(0, (char **)&stack, 4) < 0)
		return -1;
	//cprintf("inside sys_join %d\n", (uint)stack);
	//call joinHelper
	int ret = joinHelper(stack);
//	cprintf("inside sys_join %d\n", (uint)(*(char**)stack));
	return ret;
}

