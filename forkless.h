#ifndef _INCLUDE_FORKLESS_H_
#define _INCLUDE_FORKLESS_H_

#include <pthread.h>
#include <setjmp.h>

extern jmp_buf env;

#define forkless_fork2() setjmp(env), forkless_fork()

size_t forkless_fork();
int forkless_waitpid(size_t pid, int *stat, int options);
int forkless_execve (const char *path, char *const argv[],  char *const envp[]);
void forkless_exit(int rc);

#endif
