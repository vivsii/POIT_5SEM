#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>

int main()
{
	pid_t pid = getpid();
	nice(0);

	for (int i = 0; i < 10000000; i++)
	{
		printf("[OS08_06] PID = %d\n", pid);
		sleep(1);
	}
	exit(0);
}