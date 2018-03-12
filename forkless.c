#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "forkless.h"

jmp_buf env = {0};

static void *cb(void *user) {
	jmp_buf self = {0};
	size_t *selfp = (size_t*)self;
	setjmp (self);
#if 0
	size_t *ptrs = self;
	for (i = 0;i< 10; i++) {
		printf ("jiji %p\n", ptrs[i]);
		if (i == 1 || i == 2) {
			ptrs[i] = 0;
		}
	}
#endif
	size_t *envp = (size_t *)env;
	envp[1] = selfp[1];
	envp[2] = selfp[2];
	longjmp (env, 0); //pthread_self ());
	return 0;
}

pthread_t child = 0;
bool fork_twice = false;

size_t forkless_fork() {
	if (fork_twice) {
		fork_twice = false;
		return 0; //(size_t)child;
	}
	pthread_t th = NULL;
#if 0
	jmp_buf self = {0};
	if (setjmp (self) == -1) {
		fprintf (stderr, "Cannot set jmp killing thread\n");
		// TODO
	}
#endif
	printf ("NEW THREAD\n");
	pthread_create (&th, NULL, cb, NULL);
	if (th == NULL) {
		return -1;
	}
	fork_twice = true;
	// required, because forkless_fork returns first 0
	child = th;
	return (size_t)th;
}

int forkless_system() {
	return -1;
}

int forkless_execve (const char *path, char *const argv[],  char *const envp[]) {
	char fullpath[1024];
	snprintf (fullpath, sizeof (fullpath), "bin/%s", path);
	void *p = dlopen (fullpath, RTLD_GLOBAL);
	if (!p) {
		return -1;
	}
printf ("execve\n");
	int (*bin_main)(int argc, char *const argv[], char *const envp[]);
	bin_main = dlsym (p, "main");
printf ("execve2\n");
	if (!bin_main) {
		dlclose (p);
printf ("execve2: cannot find main\n");
		return -1;
	}
	int argc;
	for (argc = 0; argv[argc]; argc++);
	int rc = bin_main (argc, argv, envp);
	dlclose (p);
	pthread_exit (&rc);
	//pthread_kill (pthread_self (), 9);
	// forkless_exit (rc);
	return rc;
}

int forkless_waitpid(size_t pid, int *stat, int options) {
	pthread_t cur = (pthread_t)pid; //pthread_self();
	int *res = NULL;
	printf ("WAITPID for %p\n", (void*)(size_t)cur);
	int err = pthread_join (cur, (void*)&res);
	if (err == -1) {
		return -1;
	}
	return res? *res: -1;
}

void forkless_exit(int rc) {
	// thread child must die here
	printf ("exit\n");
}
