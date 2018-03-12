#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "forkless.h"

int main() {
	int i;
	setjmp (env);
	size_t child = forkless_fork ();
	printf ("== CHILD %p\n", (void*)(size_t)child);
	if (child == -1) {
		fprintf (stderr, "Cannot fork\n");
		return -1;
	}
	if (child) {
		int rc = forkless_waitpid (child, NULL, 0);
		printf ("RC=%d\n", rc);
	} else {
		char *const args[] = { "echo", "this", "is", "miau", "camole", NULL };
		int rc = forkless_execve ("echo", args, NULL);
		// only reachs this point if execution fails
		printf ("Cannot find execuntable %d\n", rc);
		forkless_exit (rc);
	}
	return 0;
}
