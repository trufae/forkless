#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "forkless.h"

jmp_buf forkless_env = {0};

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
	size_t *envp = (size_t *)forkless_env;
	envp[1] = selfp[1];
	envp[2] = selfp[2];
// printf ("LONGJMP %p\n", env[2]);
	longjmp (forkless_env, 0);
	return 0;
}

pthread_t child = 0;

size_t forkless_fork() {
	if (child) {
		child = 0;
		return 0;
	}
printf ("FORK DO\n");
	pthread_t th = NULL;
	pthread_create (&th, NULL, cb, NULL);
	if (th == NULL) {
		return -1;
	}
	child = th;
	return (size_t)th;
}

int forkless_system() {
	// copypaste from any libc :?
	return -1;
}

int forkless_execvp (const char *path, char *const argv[]) {
	return forkless_execve (path, argv, NULL); // or environ :?
}

int forkless_execve (const char *path, char *const argv[],  char *const envp[]) {
	char fullpath[1024];
	snprintf (fullpath, sizeof (fullpath), "bin/%s", path);
	void *p = dlopen (fullpath, RTLD_GLOBAL);
	if (!p) {
		return -1;
	}
	int (*bin_main)(int argc, char *const argv[], char *const envp[]) = dlsym (p, "main");
	if (!bin_main) {
		dlclose (p);
		return -1;
	}
	int argc;
	for (argc = 0; argv[argc]; argc++);
	// XXX. envp is universal. we need to emulate with forkless_getenv() and forkless_setenv()
	int rc = bin_main (argc, argv, envp);
	dlclose (p);
	forkless_exit (rc);
	return rc;
}

int forkless_waitpid(size_t pid, int *stat, int options) {
	pthread_t cur = (pthread_t)pid; //pthread_self();
	int *res = NULL;
	int err = pthread_join (cur, (void*)&res);
	if (err == -1) {
		return -1;
	}
	return res? (int)(size_t)res: -1;
}

void forkless_exit(int rc) {
	pthread_exit ((void*)(size_t)rc);
	// pthread_detach (pthread_self ());
	// pthread_kill (pthread_self (), 9);
}
