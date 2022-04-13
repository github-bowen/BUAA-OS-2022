#include <sys/types.h>
int glob = 6; /* external variable in initialized data */
int
main(void)
{
int var; /* automatic variable on the stack */
pid_t fpid;
var = 88;
printf("before fork\n"); /* we don't flush stdout */
if ( (fpid = fork()) < 0)
err_sys("fork error");
else if (fpid == 0) { /* child */
glob++; /* modify variables */
var++;
} else
sleep(2); /* parent */
printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
exit(0);
}
