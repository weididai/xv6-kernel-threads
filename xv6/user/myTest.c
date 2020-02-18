#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

#define PAGE (4096)
#define MAX_PROC_MEM (640 * 1024)


void myFunction(void *arg1, void *arg2){
	printf(1,"arg1 is %s\n", (char *)arg1);
	printf(2,"arg2 is %s\n", (char *)arg2);
	exit();
}


int
main(int argc, char *argv[])
{
  	printf(1, "My test starting\n");
	char *arg1 = "first argument";
	char *arg2 = "second argument";
	int pid = thread_create(myFunction, (void *)arg1, (void *)arg2);
	printf(1,"the pid of this thread is %d\n", pid);

//	char *arg3 = "third argument";
    //    char *arg4 = "forth argument";
  //      int pid2 = thread_create(myFunction, (void *)arg3, (void *)arg4);
//        printf(1,"the pid of this thread is %d\n", pid2);

//	char *arg5 = "fifth argument";
    //    char *arg6 = "sixth argument";
//      int pid3 = thread_create(myFunction, (void *)arg5, (void *)arg6);
  //      printf(1,"the pid of this thread is %d\n", pid3);
//	char *arg3 = "third argument";
  //      char *arg4 = "forth argument";
//	thread_create(myFunction, (void *)arg3, (void *)arg4);
	thread_join();
//	int ret = thread_join();
//	printf(1,"the return code of join is %d\n",ret);
	exit();

}
