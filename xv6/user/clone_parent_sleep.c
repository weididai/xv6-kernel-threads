/*
 * test for clone() and join() where the thread terminates before join() is called
 * Authors:
 * - Varun Naik, Spring 2018
 */
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 0x1000
#define check(exp, msg) if(exp) {} else {\
  global = 0xFFFFFFFF;\
  printf(1, "TEST FAILED\n");\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
  kill(ppid);\
  exit();}

int ppid = 0;
volatile int cpid = 0;
volatile int global = 0;

void
func(void *arg1, void *arg2)
{
  // Change external state
  cpid = getpid();
  check(cpid > ppid, "getpid() returned the wrong pid");
  ++global;

  exit();

  check(0, "Continued after exit");
}

int
main(int argc, char *argv[])
{
  void *stack1, *stack2;
  int pid1, pid2;

  ppid = getpid();
  check(ppid > 2, "getpid() failed");

  // Expand address space for stack
  stack1 = sbrk(PGSIZE);

	printf(1,"initial stack is %d\n", (uint)stack1);

  check(stack1 != (char *)-1, "sbrk() failed");
  check((uint)stack1 % PGSIZE == 0, "sbrk() return value is not page aligned");

  pid1 = clone(&func, NULL, NULL, stack1);
  check(pid1 > 2 && pid1 > ppid, "clone() failed");

	printf(1,"before sleep\n");

  // Sleep, so that the thread terminates before join()
  sleep(10);
	
	printf(1,"after sleep\n");

  check(cpid == pid1, "cpid is incorrect");
  check(global == 1, "global is incorrect");


printf(1,"before join \n");
  
	pid2 = join(&stack2);

printf(1,"after join stack is %d\n", (uint)stack2);

  check(pid1 == pid2, "join() returned the wrong pid");
  check(stack1 == stack2, "join() returned the wrong stack");

  printf(1, "PASSED TEST!\n");
  exit();
}
