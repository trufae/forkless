#include <stdio.h>
#include <pthread.h>
#include "forkless.h"

int main() {
int i;
	setjmp(env);
	size_t child = forkless_fork(); // fl_fork();
	printf ("== CHILD %d\n", child);
	if (child == -1) {
		fprintf (stderr, "Cannot fork\n");
		return -1;
	}
	if (child) {
		int rc = waitpid (child, NULL, 0);
		printf ("RC=%d\n", rc);
		//exit (1);
	} else {
		for (i = 0; i< 3; i++) {
			printf ("... %d\n", i);
		//	sleep(1);
		}
		// char *const args[] = { "ls", "-l", "/bin", NULL };
		char *const args[] = { "echo", "this", "is", "miau", NULL };
		int rc = forkless_execve ("echo", args, NULL);
		printf ("Child is done\n");
		forkless_exit(rc);
		pthread_detach (pthread_self());
	}
printf ("DONE\n");
	exit(0);
	return 0;
}
