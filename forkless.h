#ifndef _INCLUDE_FORKLESS_H_
#define _INCLUDE_FORKLESS_H_

#include <pthread.h>
#include <setjmp.h>

extern jmp_buf forkless_env;

#define forkless_prefork() setjmp(forkless_env)

#define forkless_fork2(x) forkless_prefork(); x = forkless_fork()

size_t forkless_fork();
int forkless_waitpid(size_t pid, int *stat, int options);
int forkless_execve (const char *path, char *const argv[],  char *const envp[]);
int forkless_execvp (const char *path, char *const argv[]);
void forkless_exit(int rc);

#endif
