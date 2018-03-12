#ifndef _INCLUDE_FORKLESS_H_
#define _INCLUDE_FORKLESS_H_

#include <pthread.h>
#include <setjmp.h>

//#define fork forkless_fork
// #define exit forkless_exit
#define waitpid forkless_waitpid
#define system forkless_system
#define execve forkless_execve

extern jmp_buf env;

// #define fl_fork() write(1,"SJ\n", 3),setjmp(NULL), forkless_fork()
#define fl_fork() setjmp(env), forkless_fork()

size_t forkless_fork();
int forkless_waitpid(size_t pid, int *stat, int options);
int forkless_execve (const char *path, char *const argv[],  char *const envp[]);
void forkless_exit(int rc);

#endif
