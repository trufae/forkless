#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "forkless.h"

int main() {
	int i;
	size_t child;
	forkless_fork2 (child);
	printf ("== CHILD %p\n", (void*)(size_t)child);
	if (child == -1) {
		fprintf (stderr, "Cannot fork\n");
		return -1;
	}
	if (child) {
		int rc = forkless_waitpid (child, NULL, 0);
		printf ("RC=%d\n", rc);
		forkless_fork2 (child);
		if (child) {
			int rc = forkless_waitpid (child, NULL, 0);
		} else {
			 char *const args[] = { "shell", "/bin/ls"};
			// char *const args[] = { "echo", "this", "is", "miau", "camole", NULL };
			forkless_execvp (*args, args);
		}
	} else {
		// char *const args[] = { "echo", "this", "is", "miau", "camole", NULL };
		char *const args[] = { "ired", "/bin/ls"}; //echo", "this", "is", "miau", "camole", NULL };
		int rc = forkless_execve (*args, args, NULL);
		// only reachs this point if execution fails
		printf ("Cannot find execuntable %d\n", rc);
		forkless_exit (rc);
printf ("SHOULD NOT BE PRINTED\n");
	}
	return 0;
}
